#include <stdint.h>
#include "../headers/combination.h"

uint64_t comb(uint64_t p, uint32_t k) {
    k = (k < p/2) ? k : p-k;
    if (k > p) {
        return 0;
    }

    uint64_t res = 1;
    for (uint32_t i = 1; i <= k; ++i) {
        res *= p--;
        res /= i;
    }

    return res;
}