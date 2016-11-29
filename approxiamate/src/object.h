//
// Created by tripack on 16-10-26.
//

#ifndef VE490_OBJECT_H
#define VE490_OBJECT_H

class Object {
public:
    virtual Object *clone() = 0;
    virtual ~Object() = 0;
};

#endif //VE490_OBJECT_H