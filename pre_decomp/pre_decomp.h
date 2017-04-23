//
// Created by tripack on 17-4-9.
//

#ifndef VE490_PRE_DECOMP_H
#define VE490_PRE_DECOMP_H

#include <string>
#include <common.h>
#include <unordered_map>
#include <bitset>
#include <memory>

class FocusedSimulationResult;

class PreDecomp {
public:

    struct DbEntry {
        size_t nInputs;
        DBitset leftMask;
        DBitset rightMask;
        DBitset discardMask;
        TTable combine;
        TTable left;
        TTable right;
        TTable function;

        DbEntry() : combine(1), left(1),
                    right(1), function(1) {}
    };

private:

    std::vector<std::vector<std::bitset<64> > > metaData;
    std::vector<std::vector<DbEntry> > data;

    static PreDecomp* instance;
    std::string libPath;
    PreDecomp(const std::string& libName = PrecompDbPath / "db.pcdb");

public:

    const std::vector<std::vector<std::bitset<64> > >& cmetaData = metaData;
    const std::vector<std::vector<DbEntry> >& cdata = data;

    const DbEntry& getMatch(const DBitset& fun, size_t inputSize) const;

    size_t nLut3() {return data[3].size();}
    size_t nLut4() {return data[4].size();}
    size_t nLut5() {return data[5].size();}
    size_t nLut6() {return data[6].size();}

    const int MINIMUM_VERSION = 102;
    static PreDecomp& getInstance();
    ~PreDecomp() noexcept {};

    PreDecomp(const PreDecomp& instance) = delete;
    PreDecomp(PreDecomp&& instance) = delete;
    PreDecomp& operator=(const PreDecomp& instance) = delete;
    PreDecomp& operator=(PreDecomp&& instance) = delete;

};

#endif //VE490_PRE_DECOMP_H
