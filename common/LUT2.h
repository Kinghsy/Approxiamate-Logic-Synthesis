//
// Created by tripack on 16-11-8.
//

#ifndef VE490_LUT2_H
#define VE490_LUT2_H

enum LUT_FUN {
    FFFF, TTTT, FFTT, TTFF,
    TFTF, FTFT, FTTF, TFFT,
    TFFF, FTFF, FFTF, FFFT,
    FTTT, TFTT, TTFT, TTTF,
    UNKNOWN_FUN
};

class LUT2 {
public:
    LUT2(LUT_FUN f) : fun(f) {}

    LUT_FUN getType() {return fun;}

private:
    LUT_FUN fun;
};

#endif //VE490_LUT2_H
