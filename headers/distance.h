#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdint.h>
#include "cluster.h"
#include "point.h"

/**
* @file distance.h
*/


/**
* @typedef *squared_distance_func_t
*
* @brief This typedef below defines the distance_func_t type, a function pointer type that groups all
* functions with the following signature :
*
*      int64_t function_name(const point_t *,const point_t *)
*
* So basically, once you've implemented squared_manhatan_distance and squared_euclidean_distance,
* you can do the following if you want:
*
*      squared_distance_func_t generic_func;
*      if (some_condition) {
*          generic_func = squared_manhattan_distance;
*      } else {
*          generic_func = squared_euclidean_distance;
*      }
*      int64_t dist = generic_func(a, b); // computes either the manhattan distance or the euclidean distance depending on the some_condition
*
* @param p1 A point of type point_t
* @param p2 A point of type point_t
*
* @see point.h
*/
typedef uint64_t (*squared_distance_func_t) (const point_t* p1, const point_t* p2);


/**
* @author Alexandre. Later modified by Gabriel and Max.
*
* @brief Computes the squared manhattan distance between 
* 
* @param p1 A point of type point_t
* @param p2 A point of type point_t
*
* @return the squared manhattan distance between 2 points
* 
* @see point.h
*/
uint64_t squared_manhattan_distance(const point_t* p1,const point_t* p2);


/**
* @author Arnaud. Later modified by Gabriel and Max.
*
* @brief  Computes the squared euclidean distance between 
*
* @param p1 A point of type point_t
* @param p2 A point of type point_t
*  
* @return the squared euclidean distance between 2 points
*
* @see point.h
*/
uint64_t squared_euclidean_distance(const point_t* p1, const point_t* p2);


/**
* @author Arnaud, Alicia and Laurence. LAter modified by Gabriel and Max.
*
* @brief  Computes the squared euclidean distance between 
*
* @param clusters An array of clusters of type cluster_t
* @param func     A function that computes the squared distance between two points of type point_t
* @param k        The number of clusters in the array of clusters
*  
* @return the squared euclidean distance between 2 points
*
* @see point.h
* @see cluster.h
*/
uint64_t distorsion(cluster_t** const clusters, squared_distance_func_t func, int64_t k);
#endif //DISTANCE_H
