//
// Created by tripack on 17-1-3.
//

#include <iostream>

using namespace std;

// Factorio
int fact(int n);
int comb(int full, int sub);
int t(int x, int c);

int main() {
    int m = 6;
    for (int i = 1; i <= m; ++i) {
        cout << i << " / " << m << " -> " << t(m, i) << endl;
    }
    return 0;
}

int fact(int n) {
    if (n == 2) return 2;
    if (n == 1) return 1;
    return n * fact(n - 1);
}

int comb(int full, int sub) {
    if (sub < full / 2) return comb(full, full - sub);
    return fact(full) / fact(sub);
}


// Returns the number of decomposing $c input
// Circuits in decomposing $x input circuit
int t(int x, int c) {
    if (c == x) return 1;
    if (c > x) return 0;
    int sum = 0;
    for (int i = 0; i <= x; ++i) {
        sum += t(x, i) + t(x, x - i);
    }
    return sum;
}
