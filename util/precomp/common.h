//
// Created by tripack on 17-3-9.
//

#ifndef VE490_COMMON_H
#define VE490_COMMON_H

#include <cinttypes>
#include <cstddef>
#include <vector>
#include <cassert>
#include <bitset>
#include <unordered_map>
#include <iostream>

#define FUN(x) std::bitset<1 << x>
typedef std::bitset<128> fun7;
typedef std::bitset<64> fun6;
typedef std::bitset<32> fun5;
typedef std::bitset<16> fun4;
typedef std::bitset<8> fun3;
typedef std::bitset<4> fun2;
typedef std::bitset<2> fun1;

template<size_t l1, size_t l2>
std::bitset<l2> upCast(std::bitset<l1> f) {
    if (l1 <= 8 * sizeof(1ull))
        return std::bitset<l2>(f.to_ullong());
    else {
        assert(0);
        return std::bitset<l2>(f.to_string());
    }
};

template<size_t k>
std::bitset<1ul << (k - 1)> cofactor(std::bitset<1 << k> f, size_t n, bool t) {
    assert(k > 1);
    std::bitset<1ul << (k - 1)> result;
    int counter = 0;
    for (size_t i = 0; i < (1 << k); i++) {
        if (std::bitset<k>(i).test(n) == t){
            result[counter++] = f[i];
        } else
            continue;
    }
    assert(counter == result.size());
    return result;
}

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



template<size_t l>
std::bitset<l> ones(size_t n = l) {
    if (n == l) {
        return std::bitset<l>().flip();
    }
    if (n <= 8 * sizeof(1ull)) {
        return std::bitset<l>((1ull << n) - 1);
    } else {
        assert(0);
    }
}

// Duplicates the first n bits and fill them in the "l"
// fillWith<16>(4, 0010 0100 0111 0110B)
//  => (0110 0110 0110 0110B)
template<size_t l>
std::bitset<l> fillWith(size_t n, std::bitset<l> f) {
    f &= ones<l>(n);
    while (l > n) {
        f |= f << n;
        n <<= 1;
    }
    return f;
}

// Does post-split masking
// Splits 0 - 2^(n - 1)
// Example m = 5, n = 3
//          0000 0000 0000 0000 1111 1111 1111 1111
//          1101 1010
// 1)       1111
// 2)                           1111
// 3)       1111                1111
// Repeats these first 32 bits on remaining sections
template<size_t k>
std::bitset<1ul << k> expansionMask(size_t m, size_t n) {
    static bool c[k + 1][k + 1] = {false};
    static std::bitset<1ul << k> ret[k + 1][k + 1];

    if (c[m][n]) return ret[m][n];

    c[m][n] = true;

    size_t halfLengthN = 1ul << (n - 1);
    size_t halfLengthM = 1ul << (m - 1);
    std::bitset<1ul << k> mask = ones<1ul << k>(halfLengthN);

    ret[m][n] = fillWith<1ul << k>(1ul << m, mask | (mask << halfLengthM));
    return ret[m][n];
}

template <size_t k, size_t kFun>
std::bitset<1ul << k> expand(std::bitset<1 << kFun> f, std::bitset<k> mask) {
    std::bitset<1ul << k> uf = upCast<1ul << kFun, 1ul << k>(f);
    assert(mask.count() == kFun);
//    std::cout << "Mask: " << mask << std::endl;
    size_t n = kFun;
    for (size_t i = k; i > 0; i--) {
        size_t halfL = 1ul << (i - 1);
        size_t halfN = 1ul << (n - 1);
//        std::cout << "uf = " << uf << std::endl;
//        std::cout << "i = " << i << std::endl;
//        std::cout << "n = " << n << std::endl;
        if (mask.test(i - 1)) {
//            std::cout << "Split." << std::endl;
            // This bit is used, we split!
            uf = uf | (uf << (halfL - halfN));
            uf &= expansionMask<k>(i, n);
            n--;
        } else {
            uf = uf | (uf << halfL);
//            std::cout << "Fill." << std::endl;
        }
    }
    return uf;
}


template <size_t l>
std::vector<std::bitset<l> >
selectWithin(size_t n, size_t ignore = 0) {
    assert (l >= n + ignore);

    static bool computed[l + 1] = {false};
    static std::vector<std::bitset<l> > ret[l + 1];

    if (computed[n] && ignore == 0) return ret[n];

    std::vector<std::bitset<l> > result;
    if (l == ignore + n || n == 0) {
        result.push_back(ones<l>(n));
        return result;
    }
    auto p1 = selectWithin<l>(n - 1, ignore + 1);
    auto p2 = selectWithin<l>(n, ignore + 1);
    for (auto t : p2) result.push_back(t);
    for (std::bitset<l> &t : p1) {
        t.flip(l - ignore - 1);
        result.push_back(t);
    }

    if (ignore == 0) {
        computed[n] = true;
        ret[n] = result;
    }
    return result;
}

template <size_t l1, size_t l2>
std::bitset<l1> transfer(std::bitset<l2> x, std::bitset<l1> mask) {
    assert(mask.count() == l2);
    int count = 0;
    for (int i = 0; i < l1; ++i) {
        if (mask.test(i)) mask[i] = x[count++];
    }
    assert(count == l2);
    return mask;
};

template<size_t k>
struct PrecompResultAttribute {
    std::bitset<k> maskLeft;       // Inputs of the left function
    std::bitset<k> maskRight;      // Inputs of the right function
    std::bitset<k> maskDiscard;    // Inputs that NOT affacting output
    std::string leftFun;
    std::string rightFun;
    std::string combine;
    size_t nLeftInput() const {return maskLeft.count();}
    size_t nRightInput() const  {return maskRight.count();}
    size_t nDiscard() const {return maskDiscard.count();}

    void print() const {
        std::cout << "LeftFun: " << leftFun << std::endl;
        std::cout << "RightFun: " << rightFun << std::endl;
        std::cout << "CombineFun: " << combine << std::endl;
        std::cout << "MaskLeft: " << maskLeft << std::endl;
        std::cout << "MaskRight: " << maskRight << std::endl;
        std::cout << "MaskDiscard:" << maskDiscard << std::endl;
    };
};

#define PRECOM_RESULT(inp) std::unordered_map<FUN(inp), PrecompResultAttribute<inp> >

template<size_t inp>
void insertToResult(
        PRECOM_RESULT(inp)& result,
        const FUN(inp)& function,
        const PrecompResultAttribute<inp>& attr) {
    if (result.count(function) == 0) {
        result.insert(std::make_pair(function, attr));
        return;
    }
    PrecompResultAttribute<inp>& prevAttr = result.at(function);
    if (prevAttr.nDiscard() < attr.nDiscard()) {
//        std::cout << "MORE_DISCARD" << std::endl;
//        std::cout << "PrevAttr: " << std::endl;
//        prevAttr.print();
//        std::cout << "Attr: " << std::endl;
//        attr.print();
        prevAttr = attr;
        return;
    }
    if ((prevAttr.combine == "XOR" || prevAttr.combine == "NXOR") &&
            (attr.combine != "XOR" && attr.combine != "NXOR")) {
        if (prevAttr.nDiscard() != attr.nDiscard()) return;
//        std::cout << "COMBINATION" << std::endl;
//        std::cout << "PrevAttr: " << std::endl;
//        prevAttr.print();
//        std::cout << "Attr: " << std::endl;
//        attr.print();
        prevAttr = attr;
        return;
    }
}


template <size_t kOut, size_t kIn1, size_t kIn2>
void enumerate(
        PRECOM_RESULT(kOut)& result,
        const PRECOM_RESULT(kIn1)& lhs,
        const PRECOM_RESULT(kIn2)& rhs) {
    assert(kOut == kIn1 + kIn2);

    // Adds 2 constants:
    PrecompResultAttribute<kOut> constAttr;
    constAttr.maskLeft.flip();
    constAttr.maskDiscard.flip();

    std::bitset<1ul << kOut> zeroFun;

    constAttr.rightFun = zeroFun.to_string();

    constAttr.leftFun = zeroFun.to_string();
    constAttr.combine = "CONST_ZERO";
    result[zeroFun] = constAttr;

    constAttr.leftFun = zeroFun.flip().to_string();
    constAttr.combine = "CONST_ONE";
    result[zeroFun] = constAttr;

    auto comb = selectWithin<kOut>(kIn1);
    for (const auto& lpair : lhs) {
//        FUN(kOut) left = upCast<kIn1, kOut>(lpair.first());
        for (const auto& rpair : rhs) {
//            FUN(kOut) right = upCast<kIn2, kOut>(rpair.first());
            for (auto c : comb) {
                PrecompResultAttribute<kOut> attr;

                auto l = expand<kOut, kIn1>(lpair.first, c);
                auto r = expand<kOut, kIn2>(rpair.first, ~c);
                auto And = l & r;
                auto nAnd = ~And;
                auto Xor = l ^ r;
                auto nXor = ~Xor;

                attr.maskLeft = c;
                attr.maskRight = ~c;
                attr.leftFun = lpair.first.to_string();
                attr.rightFun = rpair.first.to_string();
                attr.maskDiscard =
                        transfer<kOut, kIn1>(lpair.second.maskDiscard, c) |
                        transfer<kOut, kIn2>(rpair.second.maskDiscard, ~c);

                attr.combine = "AND";
                insertToResult(result, And, attr);
                attr.combine = "NAND";
                insertToResult(result, nAnd, attr);
                attr.combine = "XOR";
                insertToResult(result, Xor, attr);
                attr.combine = "NXOR";
                insertToResult(result, nXor, attr);
                attr.combine = "LEFT";
                attr.maskDiscard =
                        transfer<kOut, kIn1>(lpair.second.maskDiscard, c) | ~c;
                insertToResult(result, l, attr);
                attr.maskDiscard =
                        transfer<kOut, kIn2>(rpair.second.maskDiscard, ~c) | c;
                attr.combine = "RIGHT";
                insertToResult(result, r, attr);
            }
        }
    }
}

extern PRECOM_RESULT(1) lut1Rst;
extern PRECOM_RESULT(2) lut2Rst;

#endif //VE490_COMMON_H
