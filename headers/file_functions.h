#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <stdint.h>
#include <stdio.h>
#include "cluster.h"
#include "point.h"


/**
* @file file_functions.h
*/


/**
* @brief This function frees the memory allocated to the data
*/
void freeData(point_t** data,uint64_t n_pts);


/**
* @author Laurence and Alex. Later corrected by Gabriel.
*
* @brief This function reads the data from a file that needs to
* be processed for the k-means problem.
*
* @param file       A file pointer
* @param nbr_points A pointer to a uint64_t
*
* @return The points that need to be processed for the k-means problem. 
*
* @see point.h
*/
point_t** parse_input(FILE* file, uint64_t* nbr_points );


/**
* @author Max. Later modified by Gabriel.
*
* @brief This function writes stores the center from each cluster and their data if needed.
* The clusters can be stored into two different ways: 
* 1) Including the data that each cluster contains
* 2) Without the data that each cluster contains
* This depends on the boolean q (see below).
*
* @param cluster    The set of clusters that needs to be converted to a string
* @param k          The number of clusters in the set
* @param q          A boolean deciding if the data is stored. If q is false, the data will be stored.
* @param f          The file where each cluster has to be stored
*/
//void str_centroids(cluster_t** cluster, uint32_t k,bool q,FILE* f);
void parse_output(cluster_t** cluster, uint32_t k,bool q,FILE* f);


#endif //FILE_FUNCTIONS_H