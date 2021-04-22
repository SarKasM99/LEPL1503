#ifndef POINT_H
#define POINT_H

#include <stdint.h>
/**
* @file point.h
*/

/**
* @author group C6
*
* @typedef point_t
* 
* @brief Defines the points that are used to calculate in order to
* calculate the clusters in the k-means problem.
* 
* @var dim        
* The dimension of the point
* @var coords     
* The coordinates of the point (eg x1,x2,x3...) 
*/
typedef struct {
    uint32_t dim; 
    int64_t*  coords; 
} point_t;

#endif //POINT_H
