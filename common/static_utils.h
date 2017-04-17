//
// Created by tripack on 4/16/17.
//

#ifndef VE490_STATIC_UTILES_H
#define VE490_STATIC_UTILES_H

#include <bitset_manipulate.h>
#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os,const std::vector<T>& v) {
    for (const auto& elem : v) os << elem << " ";
    return os;
}



#endif //VE490_STATIC_UTILES_H
