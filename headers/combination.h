#ifndef COMBINATION_H
#define COMBINATION_H
#include <stdint.h>

/**
* @brief An algorithm from Knuth's "The Art of Computer Programming, 3rd Edition, Volume 2: Seminumerical Algorithms" 
* that calculates the number of combinations C(p,k)
*
* @return C(p,k)
*/
uint64_t comb(uint64_t p, uint32_t k);

#endif //COMBINATION_H