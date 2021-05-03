#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>

//Headers
#include "../headers/distance.h"
#include "../headers/point.h"
#include "../headers/cluster.h"
#include "../headers/Lloyd.h"
#include "../headers/file_functions.h"
#include "../headers/args.h"
#include "../headers/singleThread.h"
#include "../headers/combination.h"

//./kmeans -k 2 -p 7 -n 1 -f example.csv -d euclidean example.bin


int singleThread(args_t* program_arguments) {
    uint32_t k = program_arguments->k;
    uint32_t n = program_arguments->n_first_initialization_points; 
    squared_distance_func_t dist = program_arguments->squared_distance_func;
    
    bool q = program_arguments->quiet;
    
    FILE* out = program_arguments->output_stream;

    uint32_t last = k-1;

    uint64_t n_pts;  //Number of points
    point_t** data = parse_input(program_arguments->input_stream, &n_pts);
    
    if(!data){
        fprintf(stderr,"Could not read the data from the binary file\n");
        return -1;
    }

    if(n > n_pts) n = n_pts; //In case the number of points of initialisations is bigger than the number of points 
    uint32_t dim = data[0]->dim;

    //Creating a long array to stock data in a continuous array
    int64_t* coordinates = (int64_t*) malloc(n_pts*dim*sizeof(int64_t));
    if(!coordinates){fprintf(stderr,"Error during malloc\n"); return -1;}

    for(uint64_t i = 0; i<n_pts; i++){
        for(uint32_t j = 0; j<dim; j++){
            coordinates[dim*i + j] = data[i]->coords[j];
        }
    }
    
    point_t** ptrs[k];                  //Initialising pointers to points in order to test all the combinations possible ( C(n_pts,k) )
    cluster_t* clusters[k];

    //Initialisintg the set of clusters and the set of pointers.
    for(uint32_t i = 0; i<k; i++){
        clusters[i] = (cluster_t*) malloc(sizeof(cluster_t));
        if(!clusters[i]) {fprintf(stderr,"Error during malloc\n"); return -1;}

        ptrs[i] = &data[i];

		clusters[i]->size = 0;

        clusters[i]->center = (point_t*) malloc(sizeof(point_t));
        if(!clusters[i]->center) {fprintf(stderr,"Error during malloc\n"); return -1;}
    
        clusters[i]->center->dim = dim;

        clusters[i]->center->coords = (int64_t*) malloc(sizeof(int64_t)*dim);
        if(!clusters[i]->center->coords) {fprintf(stderr,"Error during malloc\n"); return -1;}

		clusters[i]->data = (point_t**) malloc(0);
    }

    //Solving k-means problem and storing the results in the csv file
	fprintf(out,"initialization centroids,distortion,centroids,clusters\n");

    if(n != k){
        uint64_t n_combinations = comb(n, k);
        uint64_t iter = 1;
        uint32_t decim = 10;
        uint64_t percent = (decim*n_combinations)/100;

        do {        
            //Initialising the combination that needs to be tested on
            for(uint32_t i = 0; i<k; i++){
                if(!memcpy(clusters[i]->center->coords,(*ptrs[i])->coords,sizeof(int64_t)*dim)) {fprintf(stderr,"Error during memcpy\n"); return -1;};
            }
            
            parse_output(clusters,k,true,out); 
            Lloyd(data, coordinates, n_pts, k, clusters, dist);

            uint64_t dis = distorsion(clusters, dist, k);            
            fprintf(out,",%"PRIu64",",dis);
            parse_output(clusters, k, q,out);
            fprintf(out,"\n"); 


            if(n_combinations >= 10 && iter >= percent && out != stdout){
                printf("%d%% done...\n",decim);
                decim += 10;
                percent = (decim*n_combinations)/100;
            } 

            iter++;
            //Testing the next combination of points
            if((*ptrs[last] != data[n-1])){
                ptrs[last]++;
                continue;
            }
            //When this statement is reached, the last pointer from 'ptrs' reached it's final position. 
            //That means all the previous pointers needs to be moved in order to check the next possible combinations
    
            uint32_t i = 0;
            while((*ptrs[i]) != data[n-k+i]){ //data[n-k-i] are the initial positions of the pointers
                i++; //Searching for the index of the first pointer that have not moved yet
            }

            ptrs[i-1]++; //Moving that pointer 
            int j;
            for (j =0; j < k-i; j++) {
                ptrs[i+j] = ptrs[i-1] + j + 1; //Placing all the pointers after the one that moved
            }   
        } while((*ptrs[0]) != data[n-k]); //While the ptr did not reach its last position (final combination)
    }

    //Testing the last combination possible
    for(uint32_t i = 0; i<k; i++){
        if(!memcpy(clusters[i]->center->coords,(*ptrs[i])->coords,sizeof(int64_t)*dim)) {fprintf(stderr,"Error during memcpy\n"); return -1;};
    }

    parse_output(clusters,k,true,out); 
    Lloyd(data, coordinates, n_pts, k, clusters, dist);

    uint64_t dis = distorsion(clusters, dist, k);            
    fprintf(out,",%"PRIu64",",dis);
    parse_output(clusters, k, q,out);
    fprintf(out,"\n"); 

    //Freeing all the data used
	freeData(data,n_pts);
	free(coordinates);

	for(uint32_t i = 0; i<k; i++){
		free(clusters[i]->center->coords);
		free(clusters[i]->center);
        free(clusters[i]->data);
        free(clusters[i]);
    }

    // close the files opened by parse_args
    if (program_arguments->input_stream != stdin) {
        fclose(program_arguments->input_stream);
    }
    if (program_arguments->output_stream != stdout) {
        fclose(program_arguments->output_stream);
    }
    
    if(out != stdout) printf("All done! :)\n");
    return 0;
}
