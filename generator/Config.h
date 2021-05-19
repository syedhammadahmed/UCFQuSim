//
// Created by SHA on 10/17/20.
//

#ifndef UCFQUSIM_CONFIG_H
#define UCFQUSIM_CONFIG_H

//#define TOTAL_PERM 120    // 5!
//#define MAPPING_THRESHOLD 120    // 5!
#define TOTAL_PERM 720    // 6!
#define MAPPING_THRESHOLD 720    // 6!
//#define TOTAL_PERM 5040    // 7!
//#define MAPPING_THRESHOLD 5040    // 7!
//#define TOTAL_PERM 40320    // 8!
//#define MAPPING_THRESHOLD 40320    // 8!
//#define TOTAL_PERM 362880    // 9!
//#define MAPPING_THRESHOLD 362880    // 9!
//#define TOTAL_PERM 3628800    // 10!
//#define MAPPING_THRESHOLD 3628800    // 10!

#define PERM_N 6
#define MAX_PERMUTATION_N 6

#define ALL_PERMUTATIONS_SCHEME false
#define INIT_MAPPING_RESTRICT_MODE false
#define K 6
#define INIT_MAPPING_DEFAULT_ONLY false
#define INIT_MAPPING_HARD_CODED_ONLY false

//#define TOTAL_PERM 3628800    // 10!
//#define MAPPING_THRESHOLD 3628800  // other mapping count after pruning

#define RANDOM_SAMPLING_INIT_MAPPINGS false
#define INIT_MAPPING_START_NODE_RANK_WISE false // buggy
#define RANDOM_SAMPLING_MAPPINGS_PRUNING false
#define RANDOM_SAMPLING_RUNS 40

#define DAG_SCHEME false

//#define INIT_MAPPING_ALL_PERMUTATIONS true
//#define INIT_MAPPING_1_BY_1 true
//
#define INIT_MAPPING_ALL_PERMUTATIONS false
#define INIT_MAPPING_1_BY_1 true
//
//#define INIT_MAPPING_ALL_PERMUTATIONS true
//#define INIT_MAPPING_1_BY_1 false
//
//#define INIT_MAPPING_ALL_PERMUTATIONS false
//#define INIT_MAPPING_1_BY_1 false

#define INIT_MAPPING_ZERO_COST_PERMUTATIONS !INIT_MAPPING_ALL_PERMUTATIONS
#define INIT_MAPPING_TOGETHER !INIT_MAPPING_1_BY_1

#define IGNORE_DIRECTION_IN_ZERO_COST_INIT_MAPPING_RESTRICTION true // generate 2x as many mappings (0-cost + 4-cost)


#define ELEMENTARY_SWAP_DECOMPOSITION true

#define CIRCUIT_BUILD_GRID true

#endif //UCFQUSIM_CONFIG_H
