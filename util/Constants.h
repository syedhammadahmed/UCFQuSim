//
// Created by hammad on 2/16/21.
//

#ifndef UCFQUSIM_CONSTANTS_H
#define UCFQUSIM_CONSTANTS_H


class Constants {
    public:
        static const int QX4_N = 5;
        static const int QX5_N = 16;

        static const int ising_model_10 = 0;
        static const int ising_model_13 = 1;
        static const int qft_10 = 2;
        static const int rd73_140 = 3;
        static const int sqn_258 = 4;
        static const int sym6_316 = 5;
        static const int sym9_146 = 6;

        constexpr static const int HARDCODED_INIT_MAPPINGS [][QX5_N] = {
//         index  0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15
                { 1, 0, 2, 3, 5, 4, 6, 7, 9, 8, 10, 11, 12, 13, 14, 15 }, // ising_model_10
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, // ising_model_13
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, // qft_10
                { 0, 1, 7, 3, 9, 5, 6, 13, 14, 15, 10, 11, 12, 4, 8, 2 }, // rd73_140
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, // sqn_258
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, // sym6_316
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } // sym9_146
        };




};


#endif //UCFQUSIM_CONSTANTS_H
