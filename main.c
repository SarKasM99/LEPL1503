#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <inttypes.h>
#include <semaphore.h>
#include <pthread.h>

//Headers
#include "headers/distance.h"
#include "headers/point.h"
#include "headers/cluster.h"
#include "headers/Lloyd.h"
#include "headers/file_functions.h"
#include "headers/args.h"
#include "headers/combination.h"

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
* @brief This function reads the arguments that are passed by during the execution and stores them in the args_t structure.
*
* @see args.h
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
        args->input_stream = fopen(argv[optind], "rb");
        if (!args->input_stream) {
            fprintf(stderr, "could not open file %s: %s\n", argv[optind], strerror(errno));
            return -1;
        }
    }

    return 0;
}
//Semaphores and mutex
pthread_mutex_t mutex;
pthread_mutex_t write;
sem_t empty;
sem_t full; 

//Declaring global variables shared between functions  
uint32_t n;                         //Number of points of initialization
uint32_t k;                         //Number of clusters
uint64_t n_pts;                     //Total number of points
uint64_t n_t;                       //Number of consumer threads
uint64_t n_combinations;            //C(n,k): number of combinations possible with k selections of n points
uint32_t dim;                       //The dimension of each point
squared_distance_func_t dist;       //The distance function (either manhattan or euclidean)
bool q;                             //A boolean that decides whether the data in each cluster needs to be written in the csv file (see the args_t structure above)
point_t** data;                     //The data that is obtained from the binary file. This variable is shared in multiple functions   
int64_t* coordinates;               //All the coordinates stored in one long array. It allows to optimise the Lloyd's algorithm without bringing massive changes to the code
FILE* out;                          //A pointer to a csv file or stdout where the results needs to be stored

point_t*** buf;         //Can be viewed as an array of arrays of point pointers. This buffer is shared with the producer and the consumers
cluster_t*** clusters;  //Can be viewed as an array of arrays of cluster pointers. Each thread takes an element from this "array" and uses to apply Lloyd's algorithm

uint32_t head = 0;      //The head of the buffer      
uint32_t tail = 0;      //The tail of the buffer
uint64_t items;         //Number of points left to process

/**
* @brief This function allows the producer to add a set of point pointers to the buffer.
* Those set of point pointers are later used by the consumer in order to apply the Lloyd's algorithm.
*/
void put(point_t** ptrs[k]){
    for(uint32_t i = 0; i < k; i++){
        buf[tail][i] = (*ptrs[i]);
    }
    tail++;
    tail = tail % n_t;
}

/**
* @brief This function allows the consumer to get a set of point pointers from the buffer and decrements the items variable.
*/
point_t** get(){
    point_t** p = buf[head];
    head++;
    head = head % n_t;
    items--;
    return p;
}

/**
* @brief This is the producer function and it is executed by the main thread.
* This function generates a combination by storing the address of each points pointers in an array 
* of pointers of point pointers (we do love pointers). Later this array is passed to the 'put' function
* where the set of point pointers are stored.
*/
void prod(){
    //Initialising pointers to point pointers in order to test all the combinations possible ( C(n,k) )
    point_t** ptrs[k]; 
    for(uint32_t i = 0; i<k; i++){
        ptrs[i] = &data[i];
    }
    
    uint64_t iter = 1;
    uint32_t decim = 10;
    uint64_t percent = (decim*n_combinations)/100;
    uint32_t last = k-1;
    do {
        sem_wait(&full);
 
        put(ptrs);                      //Puts the points of initialisation into the buffer

        sem_post(&empty);

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

        uint32_t j;
        for (j =0; j < k-i; j++) {
            ptrs[i+j] = ptrs[i-1] + j + 1; //Placing all the pointers after the one that moved
        }   
    } while((*ptrs[0]) != data[n-k]);

    return;
}


/**
* @brief This is the consumer function and it is executed by the pthreads.
* This function takes into parameter an 'id' (which is not the pthread id)
* and it represents the index of the set of clusters they need process with 
* (see the global variable above). 
*
* Each thread will be working until there are no set of points left in the buffer. 
* The global variable 'items' indicates the number of sets that needs to be tested
* with Lloyd's algorithm. Each time a thread takes a point, the variable 'items'
* is decremented.
*
* In each iteration, a thread:
* -Takes a set of point pointers from the buffer
* -Initialises the clusters with this set and applies Lloyd's algorithm
* -Writes the results down in the requested file output by the user (stdout otherwise).
*
* @param id It's the number that indicates at which index of the set of cluster pointers.
*/
void* con(void* id){
    uint32_t index = *((uint32_t*) id);
    int64_t* init_centers[k]; 

    for(uint32_t i = 0; i <k; i++){  
        init_centers[i] = malloc(dim*sizeof(int64_t));
        if(!init_centers[i]){
            {fprintf(stderr,"Error during malloc\n"); exit(EXIT_FAILURE);}
        }
    }

    while(true){
        pthread_mutex_lock(&mutex);
        
        if(items == 0){
            pthread_mutex_unlock(&mutex);
            break;
        }

        sem_wait(&empty);
        point_t** centers = get();    //Getting the set of point pointers


        //Initialising each cluster
        for(uint32_t j = 0; j< k; j++){
            memcpy(clusters[index][j]->center->coords,centers[j]->coords,dim*sizeof(int64_t)); 
            memcpy(init_centers[j],centers[j]->coords,dim*sizeof(int64_t));  
        }

        pthread_mutex_unlock(&mutex); 
        
        sem_post(&full);

        //Applying Lloyd's algorithm and calculating the distortion
        Lloyd(data, coordinates, n_pts, k, clusters[index], dist);
        uint64_t dis = distorsion(clusters[index], dist, k);

        //Writting the results
        pthread_mutex_lock(&write);

        parse_center_output(init_centers, k, dim, out);
        fprintf(out,",%"PRIu64",",dis);
        parse_output(clusters[index], k, q ,out);
        fprintf(out,"\n"); 

        pthread_mutex_unlock(&write);
    }

    for(uint32_t i = 0; i< k; i++){
        free(init_centers[i]);
    }

    return NULL;
}

/**
* @author Arnaud and Gabriel
*
* @brief Combines all the functions and solves the k-means problem by applying the Lloyd's algorithm
*/
int main(int argc, char *argv[]) {
    args_t program_arguments;   //allocate the args on the stack
    parse_args(&program_arguments, argc, argv);

    //the following fprintf (and every code already present in this skeleton) can be removed, it is just an example to show you how to use the program arguments
    fprintf(stderr, "\tnumber of threads executing the LLoyd's algoprithm in parallel: %" PRIu32 "\n", program_arguments.n_threads);
    fprintf(stderr, "\tnumber of clusters (k): %" PRIu32 "\n", program_arguments.k);
    fprintf(stderr, "\twe consider all the combinations of the %" PRIu32 " first points of the input as initializations of the Lloyd's algorithm\n", program_arguments.n_first_initialization_points);
    fprintf(stderr, "\tquiet mode: %s\n", program_arguments.quiet ? "enabled" : "disabled");
    fprintf(stderr, "\tsquared distance function: %s\n", program_arguments.squared_distance_func == squared_manhattan_distance ? "manhattan" : "euclidean");

    if(!program_arguments.input_stream){
        fprintf(stderr,"Could not open the binary file. Please check if the binary file exist or is valid\n");
        return -1;
    }
    
    //Initialising global variables
    k = program_arguments.k;
    n = program_arguments.n_first_initialization_points; 
    dist = program_arguments.squared_distance_func;
    q = program_arguments.quiet;
    n_t = program_arguments.n_threads; 
    out = program_arguments.output_stream;

    
    data = parse_input(program_arguments.input_stream, &n_pts);
    if(!data){
        fprintf(stderr,"Could not read the data from the binary file\n");
        return -1;
    }
    
    dim = data[0]->dim;

     //Creating a long array to stock data in a continuous array
    coordinates = (int64_t*) malloc(n_pts*dim*sizeof(int64_t));

    if(!coordinates){fprintf(stderr,"Error during malloc\n"); return -1;}

    for(uint64_t i = 0; i<n_pts; i++){
        for(uint32_t j = 0; j<dim; j++){
            coordinates[dim*i + j] = data[i]->coords[j];
        }
    }

    bool buf_init = true; //A boolean that tells if the buffer has been initialised or not

    if(n > n_pts) n = n_pts; //In case the number of points of initialisations is bigger than the number of points 

    if (n < program_arguments.k){
        fprintf(stderr, "Cannot generate an instance of k-means with less initialization points than needed clusters: %"PRIu32" < %"PRIu32"\n",
                n, program_arguments.k);
        fprintf(stderr,"There are %"PRIu64" points in the binary file\n",n_pts);
        return -1;
    }

    //Solving k-means problem and storing the results in the csv file
	fprintf(out,"initialization centroids,distortion,centroids,clusters\n");
    
    //If n != k, threads will be created
    if(n != k){
        n_combinations = comb(n, k);
        items = n_combinations-1;

        if(n_t > n_combinations-1) n_t = n_combinations-1;

        clusters = (cluster_t***) malloc(n_t*sizeof(cluster_t**)); 
        if(!clusters) {fprintf(stderr,"Error during malloc\n"); return -1;}

        buf = (point_t***) malloc(n_t*sizeof(point_t**));
        if(!buf) {fprintf(stderr,"Error during malloc\n"); return -1;}

        //Initialisintg the set of cluster pointers and the buffer
        for(uint32_t i = 0; i<n_t; i++){
            clusters[i] = (cluster_t**) malloc(sizeof(cluster_t*)*k);
            if(!clusters[i]) {fprintf(stderr,"Error during malloc\n"); return -1;}

            buf[i] = (point_t**) malloc(sizeof(point_t*)*k); 
            if(!buf[i]) {fprintf(stderr,"Error during malloc\n"); return -1;}

            for(uint32_t j = 0; j < k; j++){
                clusters[i][j] = (cluster_t*) malloc(sizeof(cluster_t));
                if(!clusters[i][j]) {fprintf(stderr,"Error during malloc\n"); return -1;}

		        clusters[i][j]->size = 0;
                clusters[i][j]->center = (point_t*) malloc(sizeof(point_t));
                if(!clusters[i][j]->center) {fprintf(stderr,"Error during malloc\n"); return -1;}

                clusters[i][j]->center->dim = dim;
                clusters[i][j]->center->coords = (int64_t*) malloc(sizeof(int64_t)*dim);
                if(!clusters[i][j]->center->coords) {fprintf(stderr,"Error during malloc\n"); return -1;}

		        clusters[i][j]->data = (point_t**) malloc(0);
            }
        }

        //Initialising mutex and semaphores
        if(pthread_mutex_init(&mutex,NULL) != 0){
            fprintf(stderr,"Error when initialising mutex\n");
            return -1;
        }

        if(pthread_mutex_init(&write,NULL) != 0){
            fprintf(stderr,"Error when initialising mutex\n");
            return -1;
        }

        if(sem_init(&empty, 0, 0) != 0){
            fprintf(stderr,"Error when initialising the empty semaphore\n");
            return -1;
        }

        if(sem_init(&full, 0, n_t) != 0){
            fprintf(stderr,"Error when initialising the full semaphore\n");
            return -1;
        }

        //Initialising threads
        pthread_t threads[n_t]; 
        uint32_t ids[n_t];

        uint32_t id;
        for(id = 0; id < n_t;id++){
            ids[id] = id;
            if(pthread_create(&(threads[id]),NULL,&con, &ids[id]) != 0){
                fprintf(stderr,"Error when initialising threads\n");
                return -1;
            }
        }

        //Launching the producer
        prod();

        for(uint32_t i = 0; i < n_t;i++){
            if(pthread_join(threads[i],NULL) != 0){
                fprintf(stderr,"Error when joining threads\n");
                return -1;
            }
        }

        //Destroying mutexs and semaphores
        if(pthread_mutex_destroy(&mutex) != 0 ){
            fprintf(stderr,"Error when destroying mutex\n");
            return -1;
        }

        if(pthread_mutex_destroy(&write) != 0 ){
            fprintf(stderr,"Error when destroying mutex\n");
            return -1;
        }

        if(sem_destroy(&full) != 0){
            fprintf(stderr,"Error when destroying semaphore\n");
            return -1;
        }

        if(sem_destroy(&empty) != 0){
            fprintf(stderr,"Error when destroying semaphore\n");
            return -1;
        }
    }

    //If this statement is reached, then there is only one combination possible and only the main thread is used
    else{
        clusters = (cluster_t***) malloc(sizeof(cluster_t**));
        if(!clusters) {fprintf(stderr,"Error during malloc\n"); return -1;}

        clusters[0] = (cluster_t**) malloc(k*sizeof(cluster_t*));
        if(!clusters[0]) {fprintf(stderr,"Error during malloc\n"); return -1;}

        for(uint32_t j = 0; j < k; j++){
                clusters[0][j] = (cluster_t*) malloc(sizeof(cluster_t));
                if(!clusters[0][j]) {fprintf(stderr,"Error during malloc\n"); return -1;} 

		        clusters[0][j]->size = 0;

                clusters[0][j]->center = (point_t*) malloc(sizeof(point_t));
                if(!clusters[0][j]->center) {fprintf(stderr,"Error during malloc\n"); return -1;}

                clusters[0][j]->center->dim = dim;
                clusters[0][j]->center->coords = (int64_t*) malloc(sizeof(int64_t)*dim);
                if(!clusters[0][j]->center->coords) {fprintf(stderr,"Error during malloc\n"); return -1;}

		        clusters[0][j]->data = (point_t**) malloc(0);
                if(!clusters[0][j]->data) {fprintf(stderr,"Error during malloc\n"); return -1;}
        }

        buf_init = false;
    }

    //Testing the last combination possible
    for(uint32_t i = 0; i<k; i++){
        if(!memcpy(clusters[0][i]->center->coords,data[n-k+i]->coords,sizeof(int64_t)*dim)) {fprintf(stderr,"Error during memcpy\n"); return -1;};
    }

    parse_output(clusters[0],k,true,out); 
    Lloyd(data, coordinates, n_pts, k, clusters[0], dist);

    uint64_t dis = distorsion(clusters[0], dist, k);            
    fprintf(out,",%"PRIu64",",dis);
    parse_output(clusters[0], k, q, out);
    fprintf(out,"\n"); 
    //Freeing all the data used
	freeData(data,n_pts);

	if(buf_init){
        for(uint32_t i = 0; i < n_t; i++){
            for(uint32_t j = 0; j < k; j++){
		        free(clusters[i][j]->center->coords);
		        free(clusters[i][j]->center);
                free(clusters[i][j]->data);
                free(clusters[i][j]);
            }

        free(buf[i]);
        free(clusters[i]);
        }
    } 
    
    else{
        for(uint32_t i = 0; i < k; i++){
            free(clusters[0][i]->center->coords);
		    free(clusters[0][i]->center);
            free(clusters[0][i]->data);
            free(clusters[0][i]);
        }

        free(clusters[0]);
    }

    if(buf_init) free(buf);
    free(clusters);
    free(coordinates);

    // close the files opened by parse_args
    if (program_arguments.input_stream != stdin) {
        fclose(program_arguments.input_stream);
    }

    if (program_arguments.output_stream != stdout) {
        fclose(program_arguments.output_stream);
    }
    
    if(out != stdout) printf("All done! :)\n");
    return 0;
}
