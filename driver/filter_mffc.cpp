//
// Created by tripack on 4/16/17.
//

#include "header.h"

using std::map;
using std::vector;
using std::endl;

typedef std::function<bool(const FFC&)> FfcTestFun;

void filterMffcBySize(map<BnetNodeID, FFC>& mffc) {
    auto iter= mffc.cbegin();
    while (iter != mffc.cend()) {
        if (iter->second.nodeSet.size() == 1) {
            //std::cerr << "Erased '" << iter->first << "' by size." << std::endl;
            mffc.erase(iter++);
            continue;
        }
        iter++;
    }
}

void filterMffcContainOutput(map<BnetNodeID, FFC>& mffc,
                             const vector<BnetNodeID>& output) {
    auto iter = mffc.cbegin();
    while (iter != mffc.cend()) {

        bool needremove =
                std::any_of(output.begin(), output.end(),
                            [&iter](const BnetNodeID& out) -> bool {
                                return (iter->second.nodeSet.count(out) > 0) && out != iter->first;
                            });

        if (needremove) {
            mffc.erase(iter++);
            std::cerr << "Erased '" << iter->first << "' by containing output." << endl;
            continue;
        }

        iter++;
    }
}

void filterMffcByIntersection
        (map<BnetNodeID, FFC>& mffc, const FFC& prev) {
    auto iter = mffc.cbegin();
    while (iter != mffc.cend()) {
        auto& nodeSetPrev = prev.nodeSet;
        auto& nodeSetCurr = iter->second.nodeSet;
        std::vector<BnetNodeID> intersection;
        auto it = std::set_intersection(
                nodeSetCurr.begin(), nodeSetCurr.end(),
                nodeSetPrev.begin(), nodeSetPrev.end(),
                std::back_inserter(intersection)
        );
        if (!intersection.empty()) {
            std::cout << "\tErased '" << iter->first << "' by intersection." << endl;
            mffc.erase(iter++);
            continue;
        }
        iter++;
    }
}


const FFC* findFirstFFC(map<BnetNodeID, FFC>& mffc,
                        FfcTestFun test) {
    for (const auto& elem : mffc) {
        if (test(elem.second)) return &elem.second;
    }
    return nullptr;
}



const FFC* findNextFFC(map<BnetNodeID, FFC>& mffc,
                       const FFC& prev, FfcTestFun test) {
    auto it = mffc.find(prev.name);
    if (it == mffc.end()) return nullptr;

    while (++it != mffc.end()) {
        if (test(it->second)) return &(it->second);
    }

    return nullptr;
}

const FFC*
findNextNonIntersectFFC(map<BnetNodeID, FFC>& mffc,
                        const FFC& prev, FfcTestFun test) {
    auto ptr = findNextFFC(mffc, prev, test);
    while (ptr != nullptr) {
        const auto& nodeSetPrev = prev.nodeSet;
        const auto& nodeSetCurr = ptr->nodeSet;
        std::vector<BnetNodeID> intersection;
        std::set_intersection(
                nodeSetCurr.begin(), nodeSetCurr.end(),
                nodeSetPrev.begin(), nodeSetPrev.end(),
                intersection.begin()
        );
        if (intersection.empty()) return ptr;
    }
    return nullptr;
}