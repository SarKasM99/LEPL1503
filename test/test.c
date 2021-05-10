#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <inttypes.h>

#include "../headers/distance.h"
#include "../headers/file_functions.h"
#include "../headers/Lloyd.h"


/*
 * Authors Alicia, Alexandre and Laurence
*/

/*
 * Function which create a point with a dimension (uint32_t) and an array of
 * coordinates (int64_t) given.
 * This function is used for the test Distorsion.
*/
point_t * makepoint(uint32_t dime, const int64_t coorde[]){
    point_t * point1 = (point_t *) malloc(sizeof(point_t));
    if(point1==NULL){return NULL;}
    point1->dim = dime;
    int64_t* cord = (int64_t*) malloc(dime * sizeof(int64_t));
    if(cord==NULL){return NULL;}
    for(int j = 0; j< dime; j++){
        cord[j] = (int64_t) coorde[j];
    }
    point1->coords = cord;
    return point1;
}

/*
 * Initialize 4 points used for the test functions Squared_Manhattan_Distance and Squared_Euclidian_Distance.
 */
point_t * poi(int i){

    // Initialization of the point for the distance tests
    point_t * point = (point_t *) malloc(sizeof(point_t));
    if(point==NULL){return NULL;}

    // Point1 of dimension 3 with (x=0;y=1;z=2)
    if (i==1){
        point->dim = 3;
        int64_t* cord = (int64_t*) malloc(3 * sizeof(int64_t));
        if(cord==NULL){return NULL;}
        for(int64_t j = 0; j< 3; j++){
            cord[j] = (int64_t)j;
        }
        point->coords = cord;
    }

    // Point2 of dimension 3 with (x=1;y=0;z=-1)
    else if(i==2){
    point->dim = 3;
    int64_t* cord2 = (int64_t*) malloc(3 * sizeof(int64_t));
    if(cord2==NULL){return NULL;}
    for(int64_t j = 0; j< 3; j++){
        cord2[j] = (int64_t)(-j+1);
    }
    point->coords = cord2;
    }

    // Point3 of dimension 10 with (x1=-5;x2=-3;x3=-1;x4=1;x5=3;x6=5;x7=7;x8=9;x9=11;x10=13)
    else if(i==3){
    point->dim = 10;
    int64_t* cord3 = (int64_t*) malloc(10 * sizeof(int64_t));
    if(cord3==NULL){return NULL;}
    for(int64_t j = 0; j< 10; j++){
        cord3[j] = (int64_t)(2*j-5);
    }
    point->coords = cord3;
    }

    // Point4 of dimension 10 with (x1=0;x2=1;x3=4;x4=9;x5=16;x6=25;x7=36;x8=49;x9=64;x10=81)
    else if(i==4){
    point->dim = 10;
    int64_t* cord4 = (int64_t*) malloc(10 * sizeof(int64_t));
    if(cord4==NULL){return NULL;}
    for(int64_t j = 0; j< 10; j++){
        cord4[j] = (int64_t)(j*j);
    }
    point->coords = cord4;
    }
    else {return NULL;}

    return point;
}

void test_squared_manhattan_distance1(){
    // test point 1 and point 2
    int64_t expected = 25;
    point_t* pt1 = poi(1);
    point_t* pt2 = poi(2);
    CU_ASSERT_EQUAL(squared_manhattan_distance(pt1,pt2), expected);
    free(pt1->coords);
    free(pt2->coords);
    free(pt1);
    free(pt2);
}
void test_squared_manhattan_distance2(){
    // test point 2 and point 1 (symetry)
    int64_t expected = 25;
    point_t* pt1 = poi(1);
    point_t* pt2 = poi(2);
    CU_ASSERT_EQUAL(squared_manhattan_distance(pt2,pt1), expected);
    free(pt1->coords);
    free(pt2->coords);
    free(pt1);
    free(pt2);
}

void test_squared_manhattan_distance3(){
    // test point 1 and point 1 (same points)
    int64_t expected = 0;
    point_t* pt1 = poi(1);
    CU_ASSERT_EQUAL(squared_manhattan_distance(pt1,pt1), expected);
    free(pt1->coords);
    free(pt1);
}

void test_squared_manhattan_distance4(){
    // test point 3 and point 4 (dimension 10)
    int64_t expected = (int64_t) 60025;
    point_t* pt3 = poi(3);
    point_t* pt4 = poi(4);
    CU_ASSERT_EQUAL(squared_manhattan_distance(pt3,pt4), expected);
    free(pt3->coords);
    free(pt4->coords);
    free(pt3);
    free(pt4);
}

void test_squared_euclidean_distance1(){
    // test point 1 and point 2
    int64_t expected = 11;
    point_t* pt1 = poi(1);
    point_t* pt2 = poi(2);
    CU_ASSERT_EQUAL(squared_euclidean_distance(pt1,pt2), expected);
    free(pt1->coords);
    free(pt2->coords);
    free(pt1);
    free(pt2);
}

void test_squared_euclidean_distance2(){
    // test point 2 and point 1 (symétrie)
    int64_t expected = 11;
    point_t* pt1 = poi(1);
    point_t* pt2 = poi(2);
    CU_ASSERT_EQUAL(squared_euclidean_distance(pt2,pt1), expected);
    free(pt1->coords);
    free(pt2->coords);
    free(pt1);
    free(pt2);
}

void test_squared_euclidean_distance3(){
    // test point 1 and point 1 (same points)
    int64_t expected = 0;
    point_t* pt1 = poi(1);
    CU_ASSERT_EQUAL(squared_euclidean_distance(pt1,pt1), expected);
    free(pt1->coords);
    free(pt1);
}

void test_squared_euclidean_distance4(){
    // test point 3 and point 4 (dimension 10)
    int64_t expected = (int64_t) 10573;
    point_t* pt3 = poi(3);
    point_t* pt4 = poi(4);
    CU_ASSERT_EQUAL(squared_euclidean_distance(pt3,pt4), expected);
    free(pt3->coords);
    free(pt4->coords);
    free(pt3);
    free(pt4);
}

// Authors : Alicia, Laurence & Alexandre

// Distorsion : We will first initialize 2 clusters with the function makepoint.
// Then, we will use the function distorsion on them to check the manhattan and the euclidian distorsion.

// Initialisation cluster1 with center [1,1] and points [1,1],[2,2]
cluster_t * clus1() {
    cluster_t *cluster1 = (cluster_t *) malloc(sizeof(cluster_t));
    if(cluster1==NULL){return NULL;}
    cluster1->size = 2;
    point_t ** datal = (point_t **) malloc(2 * sizeof(point_t*));
    if(datal==NULL){return NULL;}
    int64_t p1[] = {1,1};
    int64_t p2[] = {2,2};
    datal[0] = makepoint((uint32_t) 2, p1);
    datal[1] = makepoint((uint32_t) 2, p2);
    cluster1->data = datal;
    cluster1->center = makepoint((uint32_t) 2, p1);
    return cluster1;
}

// Initialisation cluster2 with center [4,5] and points [3,4],[5,7],[3,5],[5,5],[4,5]
cluster_t * clus2() {
    cluster_t *cluster2 = (cluster_t *) malloc(sizeof(cluster_t));
    if(cluster2==NULL){return NULL;}
    cluster2->size = 5;
    point_t ** datal = (point_t **) malloc(5 * sizeof(point_t*));
    if(datal==NULL){return NULL;}
    int64_t p1[] = {3,4};
    int64_t p2[] = {5,7};
    int64_t p3[] = {3,5};
    int64_t p4[] = {5,5};
    int64_t p5[] = {4,5};
    datal[0] = makepoint((uint32_t) 2, p1);
    datal[1] = makepoint((uint32_t) 2, p2);
    datal[2] = makepoint((uint32_t) 2, p3);
    datal[3] = makepoint((uint32_t) 2, p4);
    datal[4] = makepoint((uint32_t) 2, p5);
    cluster2->data = datal;
    cluster2->center = makepoint((uint32_t) 2, p5);
    return cluster2;
}

void test_manhattan_distorsion(){
    cluster_t ** dataclust = (cluster_t**) malloc(2*sizeof(cluster_t*));
    dataclust[0] = clus1();
    dataclust[1] = clus2();
    CU_ASSERT_EQUAL(distorsion(dataclust,squared_manhattan_distance,2), (uint64_t)19);
    
    free(dataclust[0]->center->coords);
    free(dataclust[0]->center);
    for(uint32_t i = 0; i<2; i++){
        free(dataclust[0]->data[i]->coords);
        free(dataclust[0]->data[i]);
    }
    free(dataclust[0]->data);
    free(dataclust[0]);

    free(dataclust[1]->center->coords);
    free(dataclust[1]->center);
    for(uint32_t i = 0; i<5; i++){
        free(dataclust[1]->data[i]->coords);
        free(dataclust[1]->data[i]);
    }
    free(dataclust[1]->data);
    free(dataclust[1]);
    free(dataclust);
}

void test_euclidian_distorsion(){
    cluster_t ** dataclust = (cluster_t**) malloc(2*sizeof (cluster_t*));
    dataclust[0] = clus1();
    dataclust[1] = clus2();
    CU_ASSERT_EQUAL(distorsion(dataclust,squared_euclidean_distance,2), (uint64_t)11);
    free(dataclust[0]->center->coords);
    free(dataclust[0]->center);
    for(uint32_t i = 0; i<2; i++){
        free(dataclust[0]->data[i]->coords);
        free(dataclust[0]->data[i]);
    }
    free(dataclust[0]->data);
    free(dataclust[0]);

    free(dataclust[1]->center->coords);
    free(dataclust[1]->center);
    for(uint32_t i = 0; i<5; i++){
        free(dataclust[1]->data[i]->coords);
        free(dataclust[1]->data[i]);
    }
    free(dataclust[1]->data);
    free(dataclust[1]);
    free(dataclust);
}

// Authors : Alicia, Alexandre & Laurence

// Test about the dimension, coordinates and number of points of the file example(dim1).bin
void test_parse_input_1() {


    FILE* fptr;
    fptr = fopen("test/example_dim1.bin","rb");
    if(!fptr) {printf("Cannot open file: error=%s\n", strerror(errno)); exit(EXIT_FAILURE);}
    
    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(data[i]->dim, (uint32_t) 1);
    }
    CU_ASSERT_EQUAL(data[0]->coords[0],(int64_t) 1);
    CU_ASSERT_EQUAL(data[1]->coords[0],(int64_t) 2);
    CU_ASSERT_EQUAL(data[2]->coords[0],(int64_t) 3);
    CU_ASSERT_EQUAL(data[3]->coords[0],(int64_t) 4);
    CU_ASSERT_EQUAL(data[4]->coords[0],(int64_t) 5);
    CU_ASSERT_EQUAL(data[5]->coords[0],(int64_t) 6);
    CU_ASSERT_EQUAL(data[6]->coords[0],(int64_t) 7);

    CU_ASSERT_EQUAL(n_pts ,(uint64_t) 7);
    freeData(data,n_pts);
    fclose(fptr);
}

// Test about the dimension, coordinates and number of points of the file example.bin
void test_parse_input_2() {

    FILE* fptr;
    fptr = fopen("test/example.bin","r");
    if(!fptr) {fprintf(stderr,"Error when opening binary file\n"); exit(EXIT_FAILURE);}
    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(data[i]->dim, (uint32_t) 2);
    }
    CU_ASSERT_EQUAL(data[0]->coords[0],(int64_t)1);
    CU_ASSERT_EQUAL(data[0]->coords[1],(int64_t)1);
    CU_ASSERT_EQUAL(data[1]->coords[0],(int64_t)2);
    CU_ASSERT_EQUAL(data[1]->coords[1],(int64_t)2);
    CU_ASSERT_EQUAL(data[2]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[2]->coords[1],(int64_t)4);
    CU_ASSERT_EQUAL(data[3]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[3]->coords[1],(int64_t)7);
    CU_ASSERT_EQUAL(data[4]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[4]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[5]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[5]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[6]->coords[0],(int64_t)4);
    CU_ASSERT_EQUAL(data[6]->coords[1],(int64_t)5);

    CU_ASSERT_EQUAL(n_pts ,(uint64_t) 7);
    freeData(data,n_pts);
    fclose(fptr);
}

// Test about the dimension, coordinates and number of points of the file example(dim3).bin
void test_parse_input_3() {
    FILE* fptr;
    fptr = fopen("test/example_dim3.bin","r");
    if(!fptr) {fprintf(stderr,"Error when opening binary file\n"); exit(EXIT_FAILURE);}

    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(data[i]->dim, (uint32_t) 3);
    }
    CU_ASSERT_EQUAL(data[0]->coords[0],(int64_t)1);
    CU_ASSERT_EQUAL(data[0]->coords[1],(int64_t)1);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)1);

    CU_ASSERT_EQUAL(data[1]->coords[0],(int64_t)2);
    CU_ASSERT_EQUAL(data[1]->coords[1],(int64_t)2);
    CU_ASSERT_EQUAL(data[1]->coords[2],(int64_t)2);

    CU_ASSERT_EQUAL(data[2]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[2]->coords[1],(int64_t)4);
    CU_ASSERT_EQUAL(data[2]->coords[2],(int64_t)4);

    CU_ASSERT_EQUAL(data[3]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[3]->coords[1],(int64_t)7);
    CU_ASSERT_EQUAL(data[3]->coords[2],(int64_t)7);

    CU_ASSERT_EQUAL(data[4]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[4]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[4]->coords[2],(int64_t)5);

    CU_ASSERT_EQUAL(data[5]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[5]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[5]->coords[2],(int64_t)5);

    CU_ASSERT_EQUAL(data[6]->coords[0],(int64_t)4);
    CU_ASSERT_EQUAL(data[6]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[6]->coords[2],(int64_t)5);

    CU_ASSERT_EQUAL(n_pts ,(uint64_t) 7);
    fclose(fptr);
    freeData(data,n_pts);
}

// Authors : Alicia, Laurence and Alexandre
// Lloyd

void test_Lloyd() {
    //Initialization of a cluster from example.bin with [1,1] and [2,2] as first centroides
    uint64_t n_pts;  //Number of points
    FILE* f = fopen("test/example.bin", "rb");
    point_t **data = parse_input(f, &n_pts);
    uint32_t dim = data[0]->dim;

    cluster_t *clusters[2]; // Two clusters

    for (uint32_t i = 0; i < 2; i++) {
        clusters[i] = (cluster_t *) malloc(sizeof(cluster_t));

        clusters[i]->size = 0;
        clusters[i]->center = (point_t *) malloc(sizeof(point_t));
        clusters[i]->center->dim = dim;
        clusters[i]->center->coords = (int64_t *) malloc(sizeof(int64_t) * dim);

        clusters[i]->data = (point_t **) malloc(0);
    }
    // Initialization of the centroides
    clusters[0]->center->coords[0] = (int64_t)1; clusters[0]->center->coords[1] = (int64_t) 1;
    clusters[1]->center->coords[0] = (int64_t)2; clusters[1]->center->coords[1] = (int64_t) 2;

    // Use of Lloyd
    int64_t* coordinates = (int64_t*) malloc(n_pts*dim*sizeof(int64_t));

    for(uint64_t i = 0; i<n_pts; i++){
        for(uint32_t j = 0; j<dim; j++){
            coordinates[dim*i + j] = data[i]->coords[j];
        }
    }

    Lloyd(data, coordinates, n_pts, 2, clusters, squared_manhattan_distance);

    // Tests about the coordinates of the centers
    CU_ASSERT_EQUAL(clusters[0]->center->coords[0], (int64_t )1);
    CU_ASSERT_EQUAL(clusters[0]->center->coords[1], (int64_t )1);
    CU_ASSERT_EQUAL(clusters[1]->center->coords[0], (int64_t )4);
    CU_ASSERT_EQUAL(clusters[1]->center->coords[1], (int64_t )5);

    // Tests about the size of the clusters
    CU_ASSERT_EQUAL(clusters[0]->size,2);
    CU_ASSERT_EQUAL(clusters[1]->size,5);

    // Tests about the coordinates of the points in the clusters
    CU_ASSERT_EQUAL(clusters[0]->data[0]->coords[0],1);
    CU_ASSERT_EQUAL(clusters[0]->data[0]->coords[1],1);
    CU_ASSERT_EQUAL(clusters[0]->data[1]->coords[0],2);
    CU_ASSERT_EQUAL(clusters[0]->data[1]->coords[1],2);

    CU_ASSERT_EQUAL(clusters[1]->data[0]->coords[0],3);
    CU_ASSERT_EQUAL(clusters[1]->data[0]->coords[1],4);
    CU_ASSERT_EQUAL(clusters[1]->data[1]->coords[0],5);
    CU_ASSERT_EQUAL(clusters[1]->data[1]->coords[1],7);
    CU_ASSERT_EQUAL(clusters[1]->data[2]->coords[0],3);
    CU_ASSERT_EQUAL(clusters[1]->data[2]->coords[1],5);
    CU_ASSERT_EQUAL(clusters[1]->data[3]->coords[0],5);
    CU_ASSERT_EQUAL(clusters[1]->data[3]->coords[1],5);
    CU_ASSERT_EQUAL(clusters[1]->data[4]->coords[0],4);
    CU_ASSERT_EQUAL(clusters[1]->data[4]->coords[1],5);

    // Free all the data used
    freeData(data,n_pts);
    free(clusters[0]->center->coords);
    free(clusters[0]->center);

    free(clusters[0]->data);
    free(clusters[0]);

    free(clusters[1]->center->coords);
    free(clusters[1]->center);

    free(clusters[1]->data);
    free(clusters[1]);
    free(coordinates);
    fclose(f);
}
// Authors : Alexandre, Laurence and Alicia

void test_output(){
    //Initialization of the cluster (same as the Lloyd test)
    uint64_t n_pts;  //Number of points
    FILE* f = fopen("test/example2.bin", "rb");
    point_t **data = parse_input(f, &n_pts);
    fclose(f);

    uint32_t dim = data[0]->dim;

    int64_t* coordinates = (int64_t*) malloc(n_pts*dim*sizeof(int64_t));

    for(uint64_t i = 0; i<n_pts; i++){
        for(uint32_t j = 0; j<dim; j++){
            coordinates[dim*i + j] = data[i]->coords[j];
        }
    }

    cluster_t *clusters[2];
    for (uint32_t i = 0; i < 2; i++) {
        clusters[i] = (cluster_t *) malloc(sizeof(cluster_t));

        clusters[i]->size = 0;
        clusters[i]->center = (point_t *) malloc(sizeof(point_t));
        clusters[i]->center->dim = dim;
        clusters[i]->center->coords = (int64_t *) malloc(sizeof(int64_t) * dim);

        clusters[i]->data = (point_t **) malloc(0);
    }
    clusters[0]->center->coords[0] = (int64_t)1; clusters[0]->center->coords[1] = (int64_t) 1;
    clusters[1]->center->coords[0] = (int64_t)2; clusters[1]->center->coords[1] = (int64_t) 2;

    // We will write in the file empty.csv the first two lines of what is written in example.csv
    // with the clusters we initialized
    FILE* fptrw = fopen("test/empty.csv", "w");
    fprintf(fptrw,"initialization centroids,distortion,centroids,clusters\n");
    parse_output(clusters,2,true,fptrw);

    Lloyd(data, coordinates, n_pts, 2, clusters, squared_manhattan_distance);
    uint64_t dis = distorsion(clusters, squared_manhattan_distance, 2);
    fprintf(fptrw,",%"PRIu64",",dis);
    parse_output(clusters, 2, false,fptrw);
    fprintf(fptrw,"\n");
    fclose(fptrw);

    // Creation of the buffers to read the files and the assert
    FILE* fptrr = fopen("test/empty.csv","r");
    FILE* fptr = fopen("test/example.csv","r");
    char buffer[100];
    char buffer2[100];
    fread(buffer,100,1,fptrr);
    fread(buffer2,100,1,fptr);
    CU_ASSERT_NSTRING_EQUAL(buffer, buffer2, 100);

    // Free and close all the data used
    freeData(data,n_pts);
    free(clusters[0]->center->coords);
    free(clusters[0]->center);

    free(clusters[0]->data);
    free(clusters[0]);

    free(clusters[1]->center->coords);
    free(clusters[1]->center);

    free(clusters[1]->data);
    free(clusters[1]);
    
    fclose(fptrr);
    fclose(fptr);
    free(coordinates);
    printf("Tests finished\n");
}

// Main function which will make all the tests
int main(){

    // Initialization of the suites
    CU_pSuite pSuite1 = NULL;
    CU_pSuite pSuiteDist = NULL;
    CU_pSuite pSuiteParse1 = NULL;
    CU_pSuite pSuiteParse2 = NULL;
    CU_pSuite pSuiteParse3 = NULL;
    CU_pSuite pSuiteLloyd = NULL;
    CU_pSuite pSuiteOutput = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite1 = CU_add_suite("Suite_1", 0, 0);
    if (NULL == pSuite1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite1, "Test manhattan1", test_squared_manhattan_distance1))
        || (NULL == CU_add_test(pSuite1, "Test manhattan1_symetrique", test_squared_manhattan_distance2))
        || (NULL == CU_add_test(pSuite1, "Test manhattan_samepoint", test_squared_manhattan_distance3))
        || (NULL == CU_add_test(pSuite1, "Test manhattan2", test_squared_manhattan_distance4))
        || (NULL == CU_add_test(pSuite1, "Test euclidean1", test_squared_euclidean_distance1))
        || (NULL == CU_add_test(pSuite1, "Test euclidean1_symetrique", test_squared_euclidean_distance2))
        || (NULL == CU_add_test(pSuite1, "Test euclidean_samepoint", test_squared_euclidean_distance3))
        || (NULL == CU_add_test(pSuite1, "Test euclidean2", test_squared_euclidean_distance4))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuiteDist = CU_add_suite("Suite_Distorsion",0, 0);
    if (NULL == pSuiteDist) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteDist, "Test Euclidian distorsion", test_euclidian_distorsion))
        || (NULL == CU_add_test(pSuiteDist, "Test Manhattan distorsion", test_manhattan_distorsion))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuiteParse1 = CU_add_suite("Suite_Parse1",0, 0); //Name of the suite, initial state, final state
    if (NULL == pSuiteParse1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParse1, "Test example(dim1).bin (dim+coords+nb points)", test_parse_input_1))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* add a suite to the registry */
    pSuiteParse2 = CU_add_suite("Suite_Parse2",0, 0); //Name of the suite, initial state, final state
    if (NULL == pSuiteParse2) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParse2, "Test example.bin (dim+coords+nb points)", test_parse_input_2))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuiteParse3 = CU_add_suite("Suite_Parse3",0, 0); //Name of the suite, initial state, final state
    if (NULL == pSuiteParse3) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParse3, "Test example(dim3).bin (dim+coords+nb points)", test_parse_input_3))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* add a suite to the registry */
    pSuiteLloyd = CU_add_suite("Suite_Lloyd",0, 0);
    if (NULL == pSuiteLloyd) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteLloyd, "Test example.bin output (Two first lines)", test_Lloyd))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuiteOutput = CU_add_suite("Suite_Output",0, 0); //Name of the suite, initial state, final state
    if (NULL == pSuiteOutput) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteOutput, "Test example.bin output (Two first lines)", test_output))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return CU_get_error();
}


