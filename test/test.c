#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>

#include "../headers/distance.h"
#include "../headers/file_functions.h"
#include "../headers/Lloyd.h"


/*
Author Alicia, Alexandre and Laurence
*/

// à refaire (pour les mallocs)
point_t * poi(int i){

    // Initialisation de différents points pour les tests de distance
    // Point1 de dimension 3 avec (x=0;y=1;z=2)
    point_t * point1 = (point_t *) malloc(sizeof(point_t));
    if(point1==NULL){return NULL;}
    point1->dim = 3;
    int64_t* cord = (int64_t*) malloc(3 * sizeof(int64_t));
    if(cord==NULL){return NULL;}
    for(int j = 0; j< 3; j++){
        cord[j] = (int64_t)j;
    }
    point1->coords = cord;

// Point2 de dimension 3 avec (x=1;y=0;z=-1)
    point_t * point2 = (point_t *) malloc(sizeof(point_t));
    if(point2==NULL){return NULL;}
    point2->dim = 3;
    int64_t* cord2 = (int64_t*) malloc(3 * sizeof(int64_t));
    if(cord2==NULL){return NULL;}
    for(int j = 0; j< 3; j++){
        cord2[j] = (int64_t)(-j+1);
    }
    point2->coords = cord2;

// Point3 de dimension 10 avec (x1=-5;x2=-3;x3=-1;x4=1;x5=3;x6=5;x7=7;x8=9;x9=11;x10=13)
    point_t * point3 = (point_t *) malloc(sizeof(point_t));
    if(point3==NULL){return NULL;}
    point3->dim = 10;
    int64_t* cord3 = (int64_t*) malloc(10 * sizeof(int64_t));
    if(cord3==NULL){return NULL;}
    for(int j = 0; j< 10; j++){
        cord3[j] = (int64_t)(2*j-5);
    }
    point3->coords = cord3;

// Point4 de dimension 10 avec (x1=0;x2=-1;x3=4;x4=-9;x5=16;x6=-25;x7=36;x8=-49;x9=64;x10=-81)
    point_t * point4 = (point_t *) malloc(sizeof(point_t));
    if(point4==NULL){return NULL;}
    point4->dim = 10;
    int64_t* cord4 = (int64_t*) malloc(10 * sizeof(int64_t));
    if(cord4==NULL){return NULL;}
    for(int j = 0; j< 10; j++){
        cord4[j] = (int64_t)(j*j*(-1)^j);
    }
    point4->coords = cord4;
    if (i==1){return point1;}
    if (i==2){return point2;}
    if (i==3){return point3;}
    if (i==4){return point4;}
    else {return NULL;}
}

void test_squared_manhattan_distance1(){
    // test point 1 et point 2
    int64_t expected = 25;
    CU_ASSERT_EQUAL(squared_manhattan_distance(poi(1),poi(2)), expected);
}
void test_squared_manhattan_distance2(){
    // test point 2 et point 1 (symétrie)
    int64_t expected = 25;
    CU_ASSERT_EQUAL(squared_manhattan_distance(poi(2),poi(1)), expected);
}

void test_squared_manhattan_distance3(){
    // test point 1 et point 1 (même point)
    int64_t expected = 0;
    CU_ASSERT_EQUAL(squared_manhattan_distance(poi(1),poi(1)), expected);
}

void test_squared_manhattan_distance4(){
    // test point 3 et point 4 (dimension 10)
    int64_t expected = 299*299;
    CU_ASSERT_EQUAL(squared_manhattan_distance(poi(3),poi(4)), expected);
}

void test_squared_manhattan_distance5(){
    //test point 2 et point 3 (pas la même dimension)
    CU_ASSERT_EQUAL(squared_manhattan_distance(poi(2),poi(3)), -1);
}

void test_squared_euclidean_distance1(){
    // test point 1 et point 2
    int64_t expected = 11;
    CU_ASSERT_EQUAL(squared_euclidean_distance(poi(1),poi(2)), expected);
}

void test_squared_euclidean_distance2(){
    // test point 2 et point 1 (symétrie)
    int64_t expected = 11;
    CU_ASSERT_EQUAL(squared_euclidean_distance(poi(2),poi(1)), expected);
}

void test_squared_euclidean_distance3(){
    // test point 1 et point 1 (même point)
    int64_t expected = 0;
    CU_ASSERT_EQUAL(squared_euclidean_distance(poi(1),poi(1)), expected);
}

void test_squared_euclidean_distance4(){
    // test point 3 et point 4 (dimension 10)
    int64_t expected = 17073;
    CU_ASSERT_EQUAL(squared_euclidean_distance(poi(3),poi(4)), expected);
}

void test_squared_euclidean_distance5(){
    //test point 2 et point 3 (pas la même dimension)
    CU_ASSERT_EQUAL(squared_euclidean_distance(poi(2), poi(3)), -1);
}

// Distorsion



//parse_input
// author : Alicia & Alexandre & Laurence

static FILE* fptr = NULL;

int init_suite_parse1(void)
{
    if (NULL == (fptr = fopen("example(dim1).bin", "rb"))) {
        return -1;
    }
    else {
        return 0;
    }
}

int init_suite_parse2(void)
{
   if (NULL == (fptr = fopen("example.bin", "rb"))) {
      return -1;
   }
   else {
      return 0;
   }
}

int init_suite_parse3(void)
{
    if (NULL == (fptr = fopen("example(dim2).bin", "rb"))) {
        return -1;
    }
    else {
        return 0;
    }
}

int clean_suite_parse(void)
{
   if (0 != fclose(fptr)) {
      return -1;
   }
   else {
      fptr = NULL;
      return 0;
   }
}

void test_parse_input_dim1() {

    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(data[i]->dim, (uint32_t) 1);
    }
}

void test_parse_input_dim2() {
    
    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(data[i]->dim, (uint32_t) 2);
    }
}

void test_parse_input_dim3() {

    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(data[i]->dim, (uint32_t) 3);
    }
}

void test_parse_input_coord1() {

    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    CU_ASSERT_EQUAL(data[0]->coords[0],(int64_t)1);
    CU_ASSERT_EQUAL(data[1]->coords[0],(int64_t)2);
    CU_ASSERT_EQUAL(data[2]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[3]->coords[0],(int64_t)4);
    CU_ASSERT_EQUAL(data[4]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[5]->coords[0],(int64_t)6);
    CU_ASSERT_EQUAL(data[6]->coords[0],(int64_t)7);
}

void test_parse_input_coord2() {
    
    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

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
}

void test_parse_input_coord3() {

    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    CU_ASSERT_EQUAL(data[0]->coords[0],(int64_t)1);
    CU_ASSERT_EQUAL(data[0]->coords[1],(int64_t)1);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)1);

    CU_ASSERT_EQUAL(data[1]->coords[0],(int64_t)2);
    CU_ASSERT_EQUAL(data[1]->coords[1],(int64_t)2);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)2);

    CU_ASSERT_EQUAL(data[2]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[2]->coords[1],(int64_t)4);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)4);

    CU_ASSERT_EQUAL(data[3]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[3]->coords[1],(int64_t)7);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)7);

    CU_ASSERT_EQUAL(data[4]->coords[0],(int64_t)3);
    CU_ASSERT_EQUAL(data[4]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)5);

    CU_ASSERT_EQUAL(data[5]->coords[0],(int64_t)5);
    CU_ASSERT_EQUAL(data[5]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)5);

    CU_ASSERT_EQUAL(data[6]->coords[0],(int64_t)4);
    CU_ASSERT_EQUAL(data[6]->coords[1],(int64_t)5);
    CU_ASSERT_EQUAL(data[0]->coords[2],(int64_t)5);

    freeData(data,n_pts);
}

void test_parse_input_nbr(){
    
    uint64_t n_pts;
    point_t **data = parse_input(fptr, &n_pts);

    CU_ASSERT_EQUAL(n_pts ,(uint64_t) 7);
    freeData(data,n_pts);
}
//Lloyd

//str_centroid

//main




int main(){

    CU_pSuite pSuite1 = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite1 = CU_add_suite("Suite_1", 0, 0); //Nom de la suite, Etat initial, Etat final
    if (NULL == pSuite1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite1, "Test manhattan1", test_squared_manhattan_distance1))
        || (NULL == CU_add_test(pSuite1, "Test manhattan1_symetrique", test_squared_manhattan_distance2))
        || (NULL == CU_add_test(pSuite1, "Test manhattan_samepoint", test_squared_manhattan_distance3))
        || (NULL == CU_add_test(pSuite1, "Test manhattan2", test_squared_manhattan_distance4))
        || (NULL == CU_add_test(pSuite1, "Test manhattan_dimensionsdifferentes", test_squared_manhattan_distance5))
        || (NULL == CU_add_test(pSuite1, "Test euclidean1", test_squared_euclidean_distance1))
        || (NULL == CU_add_test(pSuite1, "Test euclidean1_symetrique", test_squared_euclidean_distance2))
        || (NULL == CU_add_test(pSuite1, "Test euclidean_samepoint", test_squared_euclidean_distance3))
        || (NULL == CU_add_test(pSuite1, "Test euclidean2", test_squared_euclidean_distance4))
        || (NULL == CU_add_test(pSuite1, "Test euclidean_dimensionsdifferentes", test_squared_euclidean_distance5))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_pSuite pSuiteParse1 = NULL;

    /* add a suite to the registry */
    pSuiteParse1 = CU_add_suite("Suite_Parse1",init_suite_parse1, clean_suite_parse); //Nom de la suite, Etat initial, Etat final
    if (NULL == pSuiteParse1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParse1, "Test example(dim1).bin dim", test_parse_input_dim1))
        || (NULL == CU_add_test(pSuiteParse1, "Test example(dim1).bin coords", test_parse_input_coord1))
        || (NULL == CU_add_test(pSuiteParse1, "Test example(dim1).bin nbre de points", test_parse_input_nbr))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }


    CU_pSuite pSuiteParse2 = NULL;

    /* add a suite to the registry */
    pSuiteParse2 = CU_add_suite("Suite_Parse2",init_suite_parse2, clean_suite_parse); //Nom de la suite, Etat initial, Etat final
    if (NULL == pSuiteParse2) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParse2, "Test example.bin dim", test_parse_input_dim2))
    || (NULL == CU_add_test(pSuiteParse2, "Test example.bin coords", test_parse_input_coord2))
    || (NULL == CU_add_test(pSuiteParse2, "Test example.bin nbre de points", test_parse_input_nbr))
            ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite pSuiteParse3 = NULL;

    /* add a suite to the registry */
    pSuiteParse3 = CU_add_suite("Suite_Parse3",init_suite_parse3, clean_suite_parse); //Nom de la suite, Etat initial, Etat final
    if (NULL == pSuiteParse3) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParse3, "Test example(dim3).bin dim", test_parse_input_dim3))
        || (NULL == CU_add_test(pSuiteParse3, "Test example(dim3).bin coords", test_parse_input_coord3))
        || (NULL == CU_add_test(pSuiteParse3, "Test example(dim3).bin nbre de points", test_parse_input_nbr))
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


