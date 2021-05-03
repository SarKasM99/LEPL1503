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
*
* @param data   The data that needs to be freed
* @param n_pts  The number of points in the data
*
* @see point.h
*/
void freeData(point_t** data,uint64_t n_pts);

/**
* @author Laurence and Alex. Later corrected by Gabriel.
*
* @brief This function reads the data from a file that needs to
* be processed for the k-means problem. It also stores the number 
* of points in nbr_points.
*
* @param file       A file pointer
* @param nbr_points A pointer to a uint64_t, where the number of points is stored
*
* @return The points that need to be processed for the k-means problem. 
*
* @see point.h
*/
point_t** parse_input(FILE* file, uint64_t* nbr_points);

/**
* @author Max. Later modified by Gabriel.
*
* @brief This function stores the center from each cluster and their data if needed into a file.
* The clusters can be stored into two different ways: 
* 1) Including the data that each cluster contains
* 2) Without the data that each cluster contains
* This depends on the boolean q (see below).
*
* @param cluster    The set of clusters
* @param k          The number of clusters in the set
* @param q          A boolean deciding if the data is stored. If q is false, the data will be stored.
* @param f          The file where each cluster needs to be stored
*
* @see cluster.h
*/
void parse_output(cluster_t** cluster, uint32_t k,bool q,FILE* f);

/**
* @brief Similar as parse_output, it stores initial centers of each cluster in the file
*
* @param centers    Initial centers of each cluster
* @param k          The number of clusters in the set
* @param dim        The dimension of the centers
* @param f          The file where each center needs to be stored
*/
void parse_center_output(int64_t** centers, uint32_t k,uint32_t dim,FILE* f);

#endif //FILE_FUNCTIONS_H