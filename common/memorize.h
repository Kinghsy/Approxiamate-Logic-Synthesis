//
// Created by tripack on 16-12-24.
//

#ifndef VE490_MEMORIZE_H
#define VE490_MEMORIZE_H

template <class T>
class Memorized {
    bool valid;
    T data;
public:

    Memorized() {valid = false;}

    inline bool isValid() const {return valid;}
    inline const T& get() const {
        if (!valid) assert(0);
        return data;
    }

    virtual void setData(const T& d) {
        data = d;
        valid = true;
    }

    virtual void setData(T&& d) {
        data = d;
        valid = true;
    }

    virtual ~Memorized() {}
};

#endif //VE490_MEMORIZE_H
