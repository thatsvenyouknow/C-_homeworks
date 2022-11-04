#include "combinatorics.h"


uint64_t factorial(uint64_t val) {
    uint64_t count = 1;
    for (int i = 0; i < val+1; i++){
        if (i > 0){
            count *= i;
        }
    }
    return count;
}


uint64_t permutation(uint64_t val, uint64_t val2) {
    if (val<val2){
        return 0;
    } else {
        uint64_t out_perm = factorial(val)/factorial(val-val2);
        return out_perm;
    }
}


uint64_t combination(uint64_t val, uint64_t val2) {
    if (val-val2 < 0){
        return 0;
    } else if (factorial(val2)*factorial(val-val2) <= 0) {
        return 0;
    } else {
        uint64_t out_comb = factorial(val)/(factorial(val2)*factorial(val-val2));
        return out_comb;
    }
}
