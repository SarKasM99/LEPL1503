#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <endian.h>
#include <stdbool.h>

#include"../headers/file_functions.h"

/**
* @brief This function frees the memory allocated to the data
*/
void freeData(point_t** data,uint64_t n_pts){
    for(uint64_t i = 0; i<n_pts; i++){
        free(data[i]->coords);
        free(data[i]);
    }

    free(data);    
}

point_t** parse_input(FILE* file, uint64_t* nbr_points ){ 
    uint32_t predim;
    fread(&predim, sizeof(uint32_t), 1, file);     //Obtaining the dimension from the binary file
    
    uint32_t dim = be32toh(predim);

    uint64_t prenbr_points;
    fread(&prenbr_points, sizeof(uint64_t), 1, file);  //Obtaining the number of points
    *nbr_points = be64toh(prenbr_points);

    point_t** data = (point_t**) malloc( (*nbr_points) * sizeof(point_t)); //This is where the data will be stored
    if(data==NULL){return NULL;}
    
    for (uint64_t i = 0; i< (*nbr_points); i++){
        point_t* point = (point_t*) malloc(sizeof(point_t));

        if(point==NULL){
            freeData(data,i+1); 
            return NULL;
        }

        point->dim = dim;
        int64_t* cord_p = (int64_t*) malloc(dim * sizeof(int64_t));    //Allocating memory for the coordinates of the point   
        
        if(cord_p == NULL){ 
            freeData(data,i+1);
        }

        fread(cord_p, sizeof(int64_t), dim, file);

        for(uint32_t j = 0; j<  dim; j++){
            cord_p[j] = be64toh(cord_p[j]);                
        }

        point->coords = cord_p;
        data[i] = point;
    }

    return data;
}


void parse_output(cluster_t** cluster, uint32_t k,bool q,FILE* f){   
    uint32_t dim = cluster[0]->center->dim;
    fprintf(f,"\"[");

    for(uint32_t i = 0; i < k;i++){
        fprintf(f,"(");
        for(uint32_t j = 0; j < dim; j++){
            j == dim-1 ? fprintf(f,"%"PRId64"",cluster[i]->center->coords[j]) : fprintf(f,"%"PRId64", ",cluster[i]->center->coords[j]); //Writing the center of each cluster
        }
        i == k-1 ? fprintf(f,")"):fprintf(f,"), "); 
    }
    
    fprintf(f,"]\"");
    
    if(q){
        return;
    } 

    //If q is false, we write the data of each cluster
    fprintf(f,",\"[");
    for(uint32_t i = 0; i<k;i++){
        fprintf(f,"[");
        for(uint64_t j = 0; j<cluster[i]->size;j++){
            fprintf(f,"(");

            for(uint64_t m = 0; m < dim; m++){
                m == dim-1 ? fprintf(f,"%"PRId64"",cluster[i]->data[j]->coords[m]): fprintf(f,"%"PRId64", ",cluster[i]->data[j]->coords[m]); //Writting each point 
            }

            j == cluster[i]->size-1 ? fprintf(f,")"):fprintf(f,"), ");
        }
        i == k-1? fprintf(f,"]"): fprintf(f,"], ");
    }
    fprintf(f,"]\"");    
    return;
}

void parse_center_output(int64_t **centers, uint32_t k, uint32_t dim, FILE *f){
    fprintf(f,"\"[");
    for(uint32_t i = 0; i < k;i++){
        fprintf(f,"(");
        for(uint32_t j = 0; j < dim; j++){
            j == dim-1 ? fprintf(f,"%"PRId64"",centers[i][j]) : fprintf(f,"%"PRId64", ",centers[i][j]); //Writing the center of each cluster
        }
        i == k-1 ? fprintf(f,")"):fprintf(f,"), "); 
    }
    
    fprintf(f,"]\"");
}
