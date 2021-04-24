#ifndef CLUSTER_H
#define CLUSTER_H

#include <stdint.h>
#include "point.h"

/**
* @file cluster.h
*/

/**
* @author Laurence , Alicia and Arnaud. Later modified by Gabriel and Max. 
*
* @typedef cluster_t 
*
* @brief Defines the cluster that is used in 
* order to solve the k-means problem.
*
* @var center 
* The center of the cluster
* @var data 
* An array of points that belong to the cluster
* @var size 
* The number of points that belong to the cluster
*/
typedef struct {
    point_t* center; 
    point_t** data;
    int64_t size; 
} cluster_t;

#endif //CLUSTER_H
