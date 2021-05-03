#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../headers/distance.h"

//For the description of the functions and the paramaters, check the distance.h header
uint64_t squared_manhattan_distance(const point_t*  p1, const point_t*  p2)
{
    uint32_t dim = p1->dim;
    int64_t sum = 0;

    for (uint32_t i = 0; i < dim; i++){
        sum = sum + llabs(p1->coords[i] - p2->coords[i]); 
    }

    return sum*sum;

}

uint64_t squared_euclidean_distance(const point_t*  p1,const point_t*  p2)
{
    uint32_t dim = p1->dim; 
    int64_t distance = 0;

    for (uint32_t i = 0; i< dim; i++){
      int64_t x = p1->coords[i];
      int64_t y = p2->coords[i];
      distance += (x - y)*(x - y);
    }
    
    return distance;
}

uint64_t distorsion(cluster_t** const clusters, squared_distance_func_t func, int64_t k){ 

    uint64_t sum_totale = 0;
    
    for(int64_t i = 0; i < k; i++){
        int64_t sum = 0;

        for (int64_t j = 0; j < clusters[i]->size; j++){
            sum += func(clusters[i]->center, clusters[i]->data[j]);
        }

        sum_totale += sum;
    }
    
    return sum_totale; 
}

