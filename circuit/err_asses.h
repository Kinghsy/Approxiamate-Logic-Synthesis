//
// Created by tripack on 16-12-16.
//

#ifndef VE490_ERR_ASSES_H
#define VE490_ERR_ASSES_H

#include <string>
#include "interface.h"

struct BlifCompareResult {
    bool valid;
    int nInputs;
    int nSamples;
    int errorCount;
    std::string msg;

    BlifCompareResult() :
            valid(true),
            nInputs(0),
            nSamples(0),
            errorCount(0) {}
};

BlifCompareResult compareBlifs(const BlifBooleanNet& model,
                               const BlifBooleanNet& compared);

BlifCompareResult compareBlifFiles(const std::string& model,
                                   const std::string& compared);

BlifCompareResult fullCompareBlifs(const BlifBooleanNet& model,
                                   const BlifBooleanNet& compared);

BlifCompareResult sampleCompareBlifs(const BlifBooleanNet& model,
                                     const BlifBooleanNet& compared,
                                     int sampleSize = 65536);
#endif //VE490_ERR_ASSES_H
