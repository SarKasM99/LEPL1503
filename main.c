#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <inttypes.h>


//Headers
#include "headers/distance.h"
#include "headers/point.h"
#include "headers/cluster.h"
#include "headers/Lloyd.h"
#include "headers/file_functions.h"

//include pour Lloyd
//include pour les fonctions fichiers

//./kmeans -k 2 -p 7 -n 1 -f example.csv -d euclidean example.bin

/**
* @typedef args_t 
*
* @brief This structure stores all the arguments that are passed by during the execution
*
* @var input_stream
* A binary file from which the data needs to be read. By default, it is read from stdin.
* @var output_stream 
* A csv file which the results of each output of the Lloyd's algorithm needs to be stored. By default, it is stored in stdout.
* @var n_threads 
* The number of threads that needs to be used for the computation. By default, it is set to 4.
* @var k
* The number of clusters used for the computation. By default, it is set to 2.
* @var n_first_initialization_points
* The number of points that needs to be tested on for the Lloyd's algorithm. By default, it is set to k.
* @var quiet
* This boolean precises if the data from each cluster needs to be stored or not. If this q = true, the data will not be stored. By default, it is set to false.
* @var squared_distance_func
* This parameter defines the distance function that needs to be used for the computation.  By default, the squared manhattan distance is chosen.
*
* @see distance.h
*/
typedef struct {
    FILE *input_stream;
    FILE *output_stream;
    uint32_t n_threads;
    uint32_t k;
    uint32_t n_first_initialization_points;
    bool quiet;
    squared_distance_func_t squared_distance_func;
} args_t;

/**
* @brief This function prints the arguments used for the execution
*/
void usage(char *prog_name) {
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "    %s [-p n_combinations_points] [-n n_threads] [input_filename]\n", prog_name);
    fprintf(stderr, "    -k n_clusters (default value: 2): the number of clusters to compute\n");
    fprintf(stderr, "    -p n_combinations (default value: equal to k): consider the n_combinations first points present in the input to generate possible initializations for the k-means algorithm\n");
    fprintf(stderr, "    -n n_threads (default value: 4): sets the number of computing threads that will be used to execute the k-means algorithm\n");
    fprintf(stderr, "    -f output_file (default value: stdout): sets the filename on which to write the csv result\n");
    fprintf(stderr, "    -q quiet mode: does not output the clusters content (the \"clusters\" column is simply not present in the csv)\n");
    fprintf(stderr, "    -d distance (manhattan by default): can be either \"euclidean\" or \"manhattan\". Chooses the distance formula to use by the algorithm to compute the distance between the points\n");
}

/**
* @brief This function reads the arguments that are passed by during the execution and stores them in the args_t structure (see above).
*/
int parse_args(args_t *args, int argc, char *argv[]) {
    memset(args, 0, sizeof(args_t));   // set everything to 0 by default
    //the default values are the following, they will be changed depending on the arguments given to the program
    args->k = 2;
    args->n_first_initialization_points = args->k;
    args->n_threads = 4;
    args->output_stream = stdout;
    args->quiet = false;
    args->squared_distance_func = squared_manhattan_distance;
    int opt;
    while ((opt = getopt(argc, argv, "n:p:k:f:d:q")) != -1) {
        switch (opt)
        {
            case 'n':
                args->n_threads = atoi(optarg);
                if (args->n_threads <= 0) {
                    fprintf(stderr, "Wrong number of threads. Needs a positive integer, received \"%s\"\n", optarg);
                    return -1;
                }
                break;
            case 'p':
                args->n_first_initialization_points = atoi(optarg);
                if (args->n_first_initialization_points <= 0) {
                    fprintf(stderr, "Wrong number of initialization points. Needs a positive integer, received \"%s\"\n", optarg);
                    return -1;
                }
                break;
            case 'k':
                args->k = atoi(optarg);
                if (args->k <= 0) {
                    fprintf(stderr, "Wrong k. Needs a positive integer, received \"%s\"\n", optarg);
                    return -1;
                }
                break;
            case 'f':
                args->output_stream = fopen(optarg, "w");
                if (!args->output_stream) {
                    fprintf(stderr, "could not open output file %s: %s\n", optarg, strerror(errno));
                    return -1;
                }
                break;
            case 'q':
                args->quiet = true;
                break;
            case 'd':
                if (strcmp("euclidean", optarg) == 0) {
                    args->squared_distance_func = squared_euclidean_distance;
                }
                break;
            case '?':
                usage(argv[0]);
                return 1;
            default:
                usage(argv[0]);
        }
    }

    if (optind == argc) {
        args->input_stream = stdin;
    } else {
        args->input_stream = fopen(argv[optind], "r");
        if (!args->input_stream) {
            fprintf(stderr, "could not open file %s: %s\n", argv[optind], strerror(errno));
            return -1;
        }
    }

    return 0;
}

/**
* @author Arnaud and Gabriel
*
* @brief Combines all the functions described in the headers and solves the k-means problem by applying the Lloyd's algorithm
*/
int main(int argc, char *argv[]) {
    args_t program_arguments;   //allocate the args on the stack
    parse_args(&program_arguments, argc, argv);

    if (program_arguments.n_first_initialization_points < program_arguments.k) {
        fprintf(stderr, "Cannot generate an instance of k-means with less initialization points than needed clusters: %"PRIu32" < %"PRIu32"\n",
                program_arguments.n_first_initialization_points, program_arguments.k);
        return -1;
    }
    //the following fprintf (and every code already present in this skeleton) can be removed, it is just an example to show you how to use the program arguments
    fprintf(stderr, "\tnumber of threads executing the LLoyd's algoprithm in parallel: %" PRIu32 "\n", program_arguments.n_threads);
    fprintf(stderr, "\tnumber of clusters (k): %" PRIu32 "\n", program_arguments.k);
    fprintf(stderr, "\twe consider all the combinations of the %" PRIu32 " first points of the input as initializations of the Lloyd's algorithm\n", program_arguments.n_first_initialization_points);
    fprintf(stderr, "\tquiet mode: %s\n", program_arguments.quiet ? "enabled" : "disabled");
    fprintf(stderr, "\tsquared distance function: %s\n", program_arguments.squared_distance_func == squared_manhattan_distance ? "manhattan" : "euclidean");

    uint32_t k = program_arguments.k;
    uint32_t n = program_arguments.n_first_initialization_points; 
    squared_distance_func_t dist = program_arguments.squared_distance_func;
    
    bool q = program_arguments.quiet;
    
    FILE* out = program_arguments.output_stream;

    uint32_t last = k-1;

    uint64_t n_pts;  //Number of points
    point_t** data = parse_input(program_arguments.input_stream, &n_pts);
    uint32_t dim = data[0]->dim;
    
    point_t** ptrs[k];   //Initialising pointers to points in order to test all the combinations possible ( C(n_pts,k) )
    cluster_t* clusters[k];

    //Initialisintg the set of clusters and the set of pointers.
    for(uint32_t i = 0; i<k; i++){
        clusters[i] = (cluster_t*) malloc(sizeof(cluster_t));
        ptrs[i] = &data[i];

		clusters[i]->size = 0;
        clusters[i]->center = (point_t*) malloc(sizeof(point_t));
        clusters[i]->center->dim = dim;
        clusters[i]->center->coords = (int64_t*) malloc(sizeof(int64_t)*dim);

		clusters[i]->data = (point_t**) malloc(n_pts*sizeof(point_t*));
    }

    //Solving k-means problem and storing the results in the csv file
	fprintf(out,"initialization centroids,distortion,centroids,clusters\n");
    if(n != k){
        do {        
            //Initialising the combination that needs to be tested on
            for(uint32_t i = 0; i<k; i++){
                memcpy(clusters[i]->center->coords,(*ptrs[i])->coords,sizeof(int64_t)*dim);
            }
            
            parse_output(clusters,k,true,out); 
            Lloyd(data, n, k, clusters, dist);

            uint64_t dis = distorsion(clusters, dist, k);            
            fprintf(out,",%"PRIu64",",dis);
            parse_output(clusters, k, q,out);
            fprintf(out,"\n"); 

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
        } while((*ptrs[0]) != data[n_pts-k]); //While the ptr did not reach its last position (final combination)
    }

    //Testing the last combination possible
    for(uint32_t i = 0; i<k; i++){
        memcpy(clusters[i]->center->coords,(*ptrs[i])->coords,sizeof(int64_t)*dim);
    }

    parse_output(clusters,k,true,out); 
    Lloyd(data, n, k, clusters, dist);

    uint64_t dis = distorsion(clusters, dist, k);            
    fprintf(out,",%"PRIu64",",dis);
    parse_output(clusters, k, q,out);
    fprintf(out,"\n"); 

    //Freeing all the data used
	freeData(data,n_pts);
	
	for(uint32_t i = 0; i<k; i++){
		free(clusters[i]->center->coords);
		free(clusters[i]->center);
        free(clusters[i]->data);
        free(clusters[i]);
    }

    // close the files opened by parse_args
    if (program_arguments.input_stream != stdin) {
        fclose(program_arguments.input_stream);
    }
    if (program_arguments.output_stream != stdout) {
        fclose(program_arguments.output_stream);
    }
    
    return 0;
}
