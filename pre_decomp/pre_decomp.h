//
// Created by tripack on 17-4-9.
//

#ifndef VE490_PRE_DECOMP_H
#define VE490_PRE_DECOMP_H

#include <string>
#include <common.h>
#include <unordered_map>
#include <bitset>

class PreDecomp {
public:
    struct DbEntry {
        int nInputs;
        std::bitset<6> leftMask;
        std::bitset<6> rightMask;
        std::bitset<6> discardMask;
        std::string combine;
        std::bitset<64> left;
        std::bitset<64> right;
        std::bitset<64> function;
    };

private:

    std::vector<std::unordered_map<std::bitset<64>, DbEntry> > data;

    static PreDecomp* instance;
    std::string libPath;
    PreDecomp(const std::string& libName = PrecompDbPath / "db.pcdb");

public:

    DbEntry getMatch(const std::string& fun, int inputSize);

    size_t nLut3() {return data[3].size();}
    size_t nLut4() {return data[4].size();}
    size_t nLut5() {return data[5].size();}
    size_t nLut6() {return data[6].size();}

    const int MINIMUM_VERSION = 101;
    static PreDecomp& getInstance();
    ~PreDecomp() noexcept;

    PreDecomp(const PreDecomp& instance) = delete;
    PreDecomp(PreDecomp&& instance) = delete;
    PreDecomp& operator=(const PreDecomp& instance) = delete;
    PreDecomp& operator=(PreDecomp&& instance) = delete;

};


#endif //VE490_PRE_DECOMP_H
