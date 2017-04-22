//
// Created by tripack on 4/21/17.
//

#include <iostream>
#include <cstddef>

using namespace std;

int main() {
    size_t nNtDBD[10] = {0};
    size_t nDBD[10] = {0};
    size_t nSearch[10] = {0};

    auto comb = [](size_t p, size_t r) -> size_t {
        size_t prod = 1;
        for (size_t i = 1; i <= r; i++) prod *= p--;
        for (size_t i = 1; i <= r; i++) prod /= i;
        return prod;
    };

    nNtDBD[1] = 1ul;
    nDBD[1] = 4ul;
    nSearch[1] = 2ul;

    for (int i = 2; i <= 8; i ++) {
        for (int j = 1; j <= i / 2; j++) {
            nSearch[i] += 6 * comb(i, j) * nSearch[j] * nSearch[i - j];
        }
        cout << nSearch[i] << " ";
    }
    cout << endl;

    for (int i = 2; i <= 8; i ++) {
        for (int j = 1; j <= i / 2; j++) {
            nNtDBD[i] += 3 * comb(i, j) * nNtDBD[j] * nNtDBD[i - j];
        }
        cout << nNtDBD[i] << " ";
    }
    cout << endl;

    for (int i = 2; i <= 8; i ++) {
        for (int j = 1; j < i; j++) {
            nDBD[i] += comb(i, j) * nNtDBD[i - j];
        }
        cout << nDBD[i] << " ";
    }
    cout << endl;
}