//
// Created by tripack on 17-3-9.
//

#ifndef VE490_COMMON_H
#define VE490_COMMON_H

#include <cinttypes>
#include <cstddef>
#include <vector>
#include <cassert>

constexpr uint64_t bin2int(const int* a, int n) {
    return n == 0 ? 0 : bin2int(a + 1, n - 1) + ((*a) << (n - 1));
}

template <size_t n>
constexpr uint64_t bin2int(const int (&a)[n]) {
    return bin2int(a, n);
}

constexpr uint64_t lut2[] = {
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

constexpr uint64_t lut1[] = {
        bin2int({0, 1}), bin2int({1, 0}),
};

constexpr size_t nLUT1 = sizeof(lut1)/sizeof(*lut1);

//constexpr uint64_t x_and_y = bin2int({0, 0, 0, 1});
//constexpr uint64_t not_x_and_y = ~x_and_y;
//constexpr uint64_t x_xor_y = bin2int({0, 1, 1, 0});
//constexpr uint64_t not_x_xor_y = ~x_xor_y;
//constexpr uint64_t x_only = bin2int({0, 0, 1, 1});
//constexpr uint64_t y_only = bin2int({1, 1, 0, 0});

constexpr uint64_t fillWith(int m, int n, uint64_t f) {
    return m == n ? (f) : fillWith(m, n + 1, f | (f << (1 << n)));
}

constexpr uint64_t ones(int l) {
    return ((1ull << l) - 1ull);
}

constexpr uint64_t append(uint64_t f, uint64_t b, int lb) {
    return (f << lb) | b;
}


// Does post-split masking
// Splits 0 - 2^(n - 1)
// Example m = 5, n = 3
//          0000 0000 0000 0000 1111 1111 1111 1111
//          1101 1010
// 1)       1111
// 2)                           1111
// 3)       1111                1111
constexpr uint64_t expansionMask(int k, int m, int n) {
#define HALF_LENGTH_N (1 << (n - 1))
#define MASK ((1 << HALF_LENGTH_N)-1)
#define HALF_LENGTH_M (1 << (m - 1))
    return fillWith(k, m, MASK | (MASK << HALF_LENGTH_M));
#undef MASK
#undef HALF_LENGTH_N
#undef HALF_LENGTH_M
}

template <int k>
static uint64_t expand(int n, uint64_t f, int mask) {
    mask &= ones(k);
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



static std::vector<uint16_t> selectWithin(int l, int n) {
    assert (l >= n);

    std::vector<uint16_t> result;
    if (n == 0) {
        result.push_back(0ull);
        return result;
    }
    if (l == n) {
        result.push_back(ones(l));
        return result;
    }
    std::vector<uint16_t> p1 = selectWithin(l - 1, n - 1);
    std::vector<uint16_t> p2 = selectWithin(l - 1, n);
    for (auto t : p2) result.push_back(t);
    for (auto t : p1) {
        t = append(1ull, t, l - 1);
        result.push_back(t);
    }
    return result;
}

static std::vector<uint16_t> selectWithout(int l, int n) {
    std::vector<uint16_t> p = selectWithin(l, n);
    for (auto& t : p) {
        t = ~t;
        t &= ones(l);
    }
    return p;
}


struct PrecompResultAttribute {
    int nLeft; // Number of inputs of the left function
    int nRight; // Number of bits of the right function
    int nDiscard; // Number of input that NOT affacting output
    uint16_t leftPos;
    uint16_t rightPos;
};

#endif //VE490_COMMON_H
