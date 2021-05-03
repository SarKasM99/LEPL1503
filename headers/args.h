#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>
#include <stdio.h>
#include "distance.h"

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
    FILE* input_stream;
    FILE* output_stream;
    uint32_t n_threads;
    uint32_t k;
    uint32_t n_first_initialization_points;
    bool quiet;
    squared_distance_func_t squared_distance_func;
} args_t;

#endif //ARGS_H