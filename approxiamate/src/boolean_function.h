//
// Created by tripack on 16-10-26.
//

#ifndef VE490_BOOLEAN_FUNCTION_H
#define VE490_BOOLEAN_FUNCTION_H

#include "object.h"
#include <memory>
#include <stdint.h>

using std::unique_ptr;

class KMap : public Object {};


class BooleanFunction : public Object {
public:
    unique_ptr<KMap> *getKMap();
    bool operator() (int64_t input);
    BooleanFunction(char *);
};

#endif //VE490_BOOLEAN_FUNCTION_H
