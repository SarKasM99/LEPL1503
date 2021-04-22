#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../headers/Lloyd.h"

/**
* @brief This function compares centroides. It is used for the Lloyd algorithm.
*
* @returns 1 if the two arrays are the same and 0 otherwise.
*/
int compareCentroides(int64_t** c1,int64_t** c2, uint32_t k,uint32_t dim){
    if(c1[0] == NULL || c2[0] == NULL){
        return 0;
    }

    for(uint32_t i =0;i<k;i++){
        if(memcmp(c1[i],c2[i], dim*sizeof(int64_t))) return 0; 
    }

    return 1;
}

void Lloyd(point_t** points,uint64_t n,uint32_t k,cluster_t** const clusters, squared_distance_func_t dist){  
    //Initialising the centroides
    int64_t* centroides[k];      
    int64_t* old_centroides[k];

    uint32_t dim = points[0]->dim;
    bool init = true;               //This boolean is used to initialize the memory for old_centroides 

    for(uint32_t i = 0;i<k;i++){
        centroides[i] = (int64_t*) malloc(dim*sizeof(int64_t));
        memcpy(centroides[i], clusters[i]->center->coords, dim*sizeof(int64_t));
        old_centroides[i] = NULL;
    }
    
    while(!compareCentroides(centroides, old_centroides, k, dim)){
        for(uint32_t i = 0;i<k;i++){
            if(init) {
                old_centroides[i] = (int64_t*) malloc(dim*sizeof(int64_t));
            }

            memcpy(old_centroides[i],centroides[i],dim*sizeof(int64_t));
            

            clusters[i]->data = (point_t**) realloc(clusters[i]->data,0); //Reinitisialising all the points stored in each clusters
            clusters[i]->size = 0;
        }
        
        if(init) init = false;

        //Reallocating the points to their corresponding clusters
        for(uint64_t i = 0;i<n;i++){ 
            uint64_t min = dist(points[i],clusters[0]->center);
            uint32_t index = 0;                                      //Searching for the index with minimum distortion                        

            for(uint32_t j=1;j<k;j++){ //Index j starts at 1 since index 0 is already been tested above
                uint64_t res = dist(points[i],clusters[j]->center);
                if(min > res){
                    min = res;
                    index = j;
                }
            }

            clusters[index]->size++;
            clusters[index]->data = (point_t**) realloc(clusters[index]->data,clusters[index]->size*sizeof(point_t*));  //Reallocating the memory for the data stored in the cluster
            clusters[index]->data[clusters[index]->size-1] = points[i];    //Storing the pointer of the data in the cluster
        }
        
        //Recalculating the new centers
        for(uint32_t i = 0;i<k;i++){ 
            point_t* new_center;
            new_center = clusters[i]->center;
            
            for(uint32_t j = 0;j< dim;j++){  //Index j corresponds to the column of the data stored in cluster
                int64_t res = 0;

                for(uint64_t m = 0;m<clusters[i]->size;m++){ //Index m corresponds the line of the data stored in cluster
                    res += clusters[i]->data[m]->coords[j]; //Summing all the j column
                }
                new_center->coords[j] = res/clusters[i]->size;;            //Storing the new center
                centroides[i][j] = new_center->coords[j];
            }

        }
    }

    for(uint32_t i = 0;i<k;i++){
        free(centroides[i]);
        free(old_centroides[i]);
    }
}

