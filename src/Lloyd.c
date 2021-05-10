#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../headers/file_functions.h"
#include "../headers/Lloyd.h"

void Lloyd(point_t** points, int64_t* data, uint64_t n,uint32_t k,cluster_t** const clusters, squared_distance_func_t dist){
    uint32_t dim = points[0]->dim;

    int64_t* init_centroids[k];     //Storing initial centers in case something goes wrong
    for(uint32_t i = 0;i<k;i++){
        init_centroids[i] = (int64_t*) malloc(dim*sizeof(int64_t));
        if(!init_centroids[i]) {fprintf(stderr,"Error during malloc\n"); exit(EXIT_FAILURE);}
        if(!memcpy(init_centroids[i], clusters[i]->center->coords, dim*sizeof(int64_t))) {fprintf(stderr,"Error during memcpy\n"); exit(EXIT_FAILURE);}
    }
    
    //Storing old and new centroides in a continuous data form
    int64_t * centroides = (int64_t*) malloc(k*dim*sizeof(int64_t));
    if(!centroides) {fprintf(stderr,"Error during malloc\n"); exit(EXIT_FAILURE);}

    int64_t * old_centroides = (int64_t*) malloc(k*dim*sizeof(int64_t));    
    if(!old_centroides) {fprintf(stderr,"Error during malloc\n"); exit(EXIT_FAILURE);}

    for(uint32_t c = 0; c < k; c++){
        for(uint32_t j = 0; j<dim; j++){
            centroides[dim*c+j] = clusters[c]->center->coords[j];
        }
    }

    int64_t cluster_size[k];  //An array containing cluster sizes

    point_t pi; pi.dim = dim; //A point from the data
    point_t pc; pc.dim = dim; //A center from a cluster

    bool same = true;
    while (same){
        for(uint32_t c = 0; c<k; c++){
            cluster_size[c] = 0;                                     //reset the size of each cluster
            for(uint32_t j = 0; j<dim; j++){
                old_centroides[c*dim+j] = centroides[c*dim+j];
                centroides[c*dim + j] = 0;                          //reset the clusters
            }
        }
        
        for(uint64_t i =0; i < n; i++){ //get cluster appartenance
            //Initialising points
            pi.coords = &data[dim*i];              //The point we are looking for
            pc.coords = &old_centroides[0];        //The first center 

            uint64_t min = dist(&pi,&pc);
            uint32_t index = 0;                                      //Searching for the index with minimum distortion                        

            for(uint32_t c=1;c<k;c++){ //Index c starts at 1 since index 0 is already been tested above
                pc.coords = &old_centroides[dim*c];
                uint64_t res = dist(&pi,&pc);
                if(min > res){
                    min = res;
                    index = c;
                }
            }

            cluster_size[index] += 1;

            for(uint32_t j = 0; j<dim; j++){
                centroides[index*dim + j] += data[dim*i + j];  //Adding the data to the corresponding cluster to later calculate the mean
            }
        }

        for(uint32_t c = 0; c<k; c++){
            if(cluster_size[c] == 0){
                fprintf(stderr,"Two points are too close and Lloyd's algorithm was not applied successfuly\n");
                fprintf(stderr,"Those are the points of initialisation that failed:\n");
                parse_center_output(init_centroids, k, dim, stderr);
                fprintf(stderr,"\n");
                exit(EXIT_FAILURE);
            }

            for(uint32_t j = 0; j<dim; j++){
                centroides[c*dim + j] = centroides[c*dim + j] / cluster_size[c]; //Calculate the new centers to the corresponding cluster
            }
        }
        
        same = memcmp(centroides,old_centroides, k*dim*sizeof(int64_t));
    }
    
    //Now storing the center and allocating memory for the data of each cluster
    for(uint32_t c = 0; c<k; c++){

        for(uint32_t j = 0; j<dim; j++){
            clusters[c]->center->coords[j] = centroides[dim*c+j];
        }

        clusters[c]->size = cluster_size[c];

        clusters[c]->data = (point_t**) realloc(clusters[c]->data,clusters[c]->size*sizeof(point_t*));
        if(!clusters[c]->data) {fprintf(stderr,"Error during realloc\n"); exit(EXIT_FAILURE);}
        clusters[c]->center->dim = dim;
        
        cluster_size[c] = 0;
    }

    //Storing the new data into each cluster
    for(uint64_t i = 0; i<n; i++){
        uint64_t min = dist(points[i],clusters[0]->center);
        uint32_t index = 0;                                      //Searching for the index with minimum distortion                        

        for(uint32_t j=1;j<k;j++){ //Index j starts at 1 since index 0 is already been tested above
            uint64_t res = dist(points[i],clusters[j]->center);
            if(min > res){
                min = res;
                index = j;
            }
        }

        clusters[index]->data[cluster_size[index]] = points[i];
        cluster_size[index]++;
    }
    
    for(uint32_t i = 0;i<k;i++){
        free(init_centroids[i]);
    }

    free(centroides);
    free(old_centroides);
}
