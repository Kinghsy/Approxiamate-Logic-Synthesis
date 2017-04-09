//
// Created by tripack on 17-4-9.
//

#include "pre_decomp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using std::ifstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::cerr;
using std::cout;
using std::endl;

PreDecomp* PreDecomp::instance = nullptr;

static int unPow(int x) {
    switch (x) {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 16: return 4;
        case 32: return 5;
        case 64: return 6;
        default:
            assert(0);
    }
}

PreDecomp::PreDecomp(const std::string &libName) {
    this->libPath = libName;
    ifstream db(libPath);
    int version = 0;
    db >> version;
    if (!db || version < MINIMUM_VERSION) {
        cerr << "Cannot determine database version or database out of date." << endl;
        cerr << "The mininum supported database version is " << MINIMUM_VERSION << endl;
        cerr << "Please run 'precomp_rel' to regenerate the database" << endl;
        exit(-1);
    }
    data.resize(7); // Currently 6
    string line;
    while (std::getline(db, line)) {
        std::stringstream ss(line);
        DbEntry entry;

        ss >> entry.nInputs >> entry.function;
        ss >> entry.leftMask >> entry.rightMask
           >> entry.left >> entry.right
           >> entry.combine >> entry.discardMask;

        if (!ss) {
            if (line != "") {
                cerr << "Cannot parse line:\n" << line << endl;
                cerr << "Got:"
                     << entry.nInputs << "\t" << entry.function << "\t"
                     << entry.leftMask.to_string() << "\t"
                     << entry.rightMask.to_string() << "\t"
                     << entry.left << "\t"
                     << entry.right << "\t"
                     << entry.combine << "\t"
                     << entry.discardMask.to_string() << "\t";
                cerr << endl;
            }
            continue;
        }

        entry.nInputs = unPow(entry.nInputs);
        data[entry.nInputs][entry.function] = entry;
    }
    db.close();
}

PreDecomp &PreDecomp::getInstance() {
    if (instance == nullptr) {
        instance = new PreDecomp;
    }
    return *instance;
}

PreDecomp::~PreDecomp() {}

PreDecomp::DbEntry PreDecomp::getMatch(const std::string &funStr, int inputSize) {
    auto& dataSet = data.at(inputSize);
    std::bitset<64> function(funStr);
    std::bitset<64> validMask((1ull << (1ull << inputSize)) - 1);
    function &= validMask;
    size_t minError = 64;
    const DbEntry* minEntry = nullptr;
    for (const auto& e: dataSet) {
        const auto& entry = e.second;
        size_t error = (entry.function ^ function).count();
        if (error < minError) {
            minEntry = &entry;
            minError = error;
        }
    }
    return *minEntry;
}
