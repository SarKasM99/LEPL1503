#ifndef LLOYD_H
#define LLOYD_H

#include <stdint.h>
#include "cluster.h"
#include "point.h"
#include "distance.h"

/**
* @file Lloyd.h
*/

/**
* @author Arnaud and Gaby
*
* @brief This function implements the Lloyd algorithm. 
* It takes a set of clusters, assigns the points to their 
* closest cluster and then computes the new centroids of each cluster. 
* The centroids are stored in the field (center) of each cluster.
*
* @param points     The set of points
* @param data       The set of coordinates stored in a continuous array
* @param n          The number of points
* @param clusters   The set of clusters
* @param k          The number of clusters 
* @param dist       A function that computes the distance between two points
*
* @see cluster.h
* @see point.h
* @see distance.h
*/
void Lloyd(point_t** points,int64_t* data, uint64_t n,uint32_t k,cluster_t** const clusters, squared_distance_func_t dist);


#endif //LLOYD_H