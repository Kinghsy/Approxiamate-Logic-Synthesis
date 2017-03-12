//
// Created by tripack on 17-3-9.
//

#ifndef VE490_COMMON_H
#define VE490_COMMON_H

#include <cinttypes>
#include <cstddef>

constexpr uint32_t bin2int(const int* a, int n) {
    return n == 0 ? 0 : bin2int(a + 1, n - 1) + ((*a) << (n - 1));
}

template <size_t n>
constexpr uint32_t bin2int(const int (&a)[n]) {
    return bin2int(a, n);
}

constexpr uint32_t lut2[] = {
        bin2int({0, 0, 0, 0}), bin2int({0, 0, 0, 1}),
        bin2int({0, 0, 1, 0}), bin2int({0, 0, 1, 1}),
        bin2int({0, 1, 0, 0}), bin2int({0, 1, 0, 1}),
        bin2int({0, 1, 1, 0}), bin2int({0, 1, 1, 1}),
        bin2int({1, 0, 0, 0}), bin2int({1, 0, 0, 1}),
        bin2int({1, 0, 1, 0}), bin2int({1, 0, 1, 1}),
        bin2int({1, 1, 0, 0}), bin2int({1, 1, 0, 1}),
        bin2int({1, 1, 1, 0}), bin2int({1, 1, 0, 1})
};

constexpr size_t nLUT2 = sizeof(lut2)/sizeof(*lut2);

constexpr uint32_t lut1[] = {
        bin2int({0, 1}), bin2int({1, 0}),
};

constexpr size_t nLUT1 = sizeof(lut1)/sizeof(*lut1);

//constexpr uint32_t x_and_y = bin2int({0, 0, 0, 1});
//constexpr uint32_t not_x_and_y = ~x_and_y;
//constexpr uint32_t x_xor_y = bin2int({0, 1, 1, 0});
//constexpr uint32_t not_x_xor_y = ~x_xor_y;
//constexpr uint32_t x_only = bin2int({0, 0, 1, 1});
//constexpr uint32_t y_only = bin2int({1, 1, 0, 0});

constexpr uint32_t fillWith(int m, int n, uint32_t f) {
    return m == n ? (f) : fillWith(m, n + 1, f | (f << (1 << n)));
}


// Does post-split masking
// Splits 0 - 2^(n - 1)
// Example m = 5, n = 3
//          0000 0000 0000 0000 1111 1111 1111 1111
//          1101 1010
// 1)       1111
// 2)                           1111
// 3)       1111                1111
constexpr uint32_t expansionMask(int k, int m, int n) {
#define HALF_LENGTH_N (1 << (n - 1))
#define MASK ((1 << HALF_LENGTH_N)-1)
#define HALF_LENGTH_M (1 << (m - 1))
    return fillWith(k, m, MASK | (MASK << HALF_LENGTH_M));
#undef MASK
#undef HALF_LENGTH_N
#undef HALF_LENGTH_M
}

template <int k>
static uint32_t expand(int n, uint32_t f, int mask) {
    for (int i = k; i > 0; i--) {
        int halfL = 1 << (i - 1);
        int halfN = 1 << (n - 1);
        if ((mask & (1 << (i - 1))) > 0) {
            // This bit is used, we split!
            f = f | (f << (halfL - halfN));
            f &= expansionMask(k, i, n);
            n--;
        } else {
            f = f | (f << halfL);
        }
    }
    return f;
}

void do3input(uint8_t* lut3);
void do4input(uint8_t* lut4,
              const uint8_t* lut3, const int nLut3);

void do5input(uint8_t* lut5,
              const uint16_t* lut4, const int nLut4,
              const uint8_t* lut3, const int nLut3);
#endif //VE490_COMMON_H
