LEPL1503 - k-means problem - group C6 

# Purpose
This project solves a clustering problem, more precisely it solves the k-means problem. 

# How it works?

The program first reads the data from a binary file, where: \
-The first number is an **unsigned 32 bits int** that represents the dimension of the points used \
-The second number is an **unsigned 64 bits int** that represents the number of points in the data \
-The rest is a succession of **signed 64 bits int** that gives the coordinates of each point \
**NB**: The data in the binary file needs to be encoded in big endian order. The program will later convert to host byte order. 

Then the program applies the Lloyd's algorithm testing every combinations of initialisation of clusters possible among the n initialisation of points that is specified by the user (more details below). 

After solving the clustering problem with Lloyd's algorithm, the program stores the result in a *CSV* file of each initialisation. Note that the program does not store in any particular order between the lines since the program uses threads in order to solve the problem faster. Here is an example of how an initialisation is stored: \
-First line is always: \
`initialization centroids,distortion,centroids,clusters` \
-And the rest are the results, where the each quotation represents (respectively) initialization centroids, distortion, centroids and data in each cluster (respectively to the order of centroids) \
`"[(1, 1), (2, 2)]",11,"[(1, 1), (4, 5)]","[[(1, 1), (2, 2)], [(3, 4), (5, 7), (3, 5), (5, 5), (4, 5)]]"`

# How to use? 
Use our makefile in order to simplify the compilation process:
* `make kmeans` - compiles and creates an executable file to solve the clustering problem 
* `make tests` - does kmeans, then compiles our units tests and executes them 
* `make clean` - removes all executables and object files 
* `make memcheck` - launches valgrind memory checks with helgrind

Here are the parameters that can be used with the executable file (kmeans): \
**-q** if used, then the data in each cluster (the last column) in the csv file will not be shown \
**-k n_clusters** (default: 2) specifies the numbers of clusters  \
**-p n_combinations** (default: same as n_clusters) specifies the first number of points that needs to be tested for the clustering problem. \ For example, if n_clusters = 2 and n_combinations = 7, then the program will test C(n_combinations,n_clusters) combinations.  \
**-n threads** (default: 4) specifies the numbers of threads that applies Lloyd's algorithm \
**-d distance_metric** (default: "manhattan") it's either "manhattan" or "euclidean". Specifies the distance formula needs to be used \
**-f output_file** (default: *stdout*) the path of the csv file where the results needs to be written \
**input_filename** (default: *stdin*) the path of the binary file that contains the data that needs to be processed. 

# Structures 
The structures used in the programm are described below :
```c
typedef struct { 
    FILE* input_stream;                             //Input file pointer
    FILE* output_stream;                            //Output file pointer
    uint32_t n_threads;                             //Number of threads that applies the algorithm
    uint32_t k;                                     //Number of clusters
    uint32_t n_first_initialization_points;         //Number of points of initialisations
    bool quiet;                                     //A boolean that determines whether the set of points needs to be written in the output file
    squared_distance_func_t squared_distance_func;  //A generic type defined in distance.c to decide which distance function to apply
} args_t; 
```
```c
typedef struct { 
    uint32_t dim;       //dimension of the point 
    int64_t*  coords;   //pointer to the coordinates of the point 
} point_t;
```
```c
typedef struct { 
    point_t* center;    //the center of the cluster
    point_t** data;     //the set of points that belongs to the cluster
    int64_t size;       //number of points in the cluster 
} cluster_t;
```

# Threads
The solution is based on the 'consumer and procuder' strategy. The producer generates combinations of points (C(n,k) where
n is the number of points of initialisation and k is the number of clusters). The consumers takes those points from the buffer, applies Lloyd's algorithm and writes the resutls in the requested output file. 

The producer and the consumers communicates with a semaphor in order to give signals when the buffer is either full or empty. The producer will not stop producing until it generated all the points. Each consumer thread will not stop working until there are no points left. 

The concurrency between producers are controlled by two mutex: \
`pthread_mutex_t mutex;` \
`pthread_mutex_t write;`


Mutex `write` locks the process of writting in the output file. Therefore only one thread at a time can write in this file. 

Mutex `mutex` locks the process of taking a point from the buffer. First it locks and checks if there is any elements left in the buffer 
(the number of items in the buffer is calculated by applying a combination algorithm and then decremented each time a thread takes a point). 

If there still an item to take, the consumer will communicate with the producer (with `sem_wait(&empty)`) to check if the set of points that the  consumer wishes to obtain is ready. After the consumer got his points, he unlocks the mutex and signals the producer once he finished copying his set of points. 

If there is no item left to take, the consumer unlocks the mutex and exit the while loop.

# Perfomance
Here are some performance tests to show the speed-up of the threads. Those test were done on a **Raspberry Pi** with a binary file that contains 200 points.

Tests -k 2 -p 25 -d euclidian:
- 1 thread  : 0.1752s
- 2 threads : 0.0966s
- 3 threads : 0.065s
- 4 threads : 0.0562

Tests -k 2 -p 85 -d euclidian:
- 1 thread  : 2.0136s
- 2 threads : 1.0176s
- 3 threads : 0.7242s
- 4 threads : 0.5342s

Tests -k 2 -p 200 -d euclidian:
- 1 thread  : 11.0078s
- 2 threads : 5.6064s
- 3 threads : 3.7928s
- 4 threads : 2.9104s


