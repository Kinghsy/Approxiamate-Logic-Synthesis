//
// Created by tripack on 4/16/17.
//

#include "header.h"

using std::map;
using std::vector;
using std::endl;

typedef std::function<bool(const FFC&)> FfcTestFun;

void filterMffcBySize(map<BnetNodeID, FFC>& mffc) {
    for (auto iter = mffc.begin(); iter != mffc.end();) {
        bool needDelete = false;
        needDelete |= (iter->second.inputNode.size() < 3);
        needDelete |= (iter->second.inputNode.size() > 20);
        if (needDelete) {
            //std::cerr << "Erased '" << iter->first << "' by size." << std::endl;
            iter = mffc.erase(iter);
        } else {
            ++iter;
        }
    }
}

void filterMffcContainOutput(map<BnetNodeID, FFC>& mffc,
                             const vector<BnetNodeID>& output) {
    std::vector<decltype(mffc.begin())> removeSet;
    for (auto iter = mffc.begin(); iter != mffc.end();) {
        bool needremove =
                std::any_of(output.begin(), output.end(),
                            [&iter](const BnetNodeID& out) -> bool {
                                return (iter->second.nodeSet.count(out) > 0) && out != iter->first;
                            });
        if (needremove) removeSet.push_back(iter);
        ++iter;
        for (auto i : removeSet) {
            std::cerr << "Erased '" << iter->first << "' by containing output." << endl;
            mffc.erase(i);
        }
    }
}

void filterMffcByIntersection
        (map<BnetNodeID, FFC>& mffc, const FFC& prev) {
    std::vector<decltype(mffc.begin())> removeSet;
    for (auto iter = mffc.begin(); iter != mffc.end();) {
        auto& nodeSetPrev = prev.nodeSet;
        auto& nodeSetCurr = iter->second.nodeSet;
        std::vector<BnetNodeID> intersection;
        auto it = std::set_intersection(
                nodeSetCurr.begin(), nodeSetCurr.end(),
                nodeSetPrev.begin(), nodeSetPrev.end(),
                std::back_inserter(intersection)
        );
        if (!intersection.empty()) removeSet.push_back(iter);
        ++iter;
    }
    for (auto i : removeSet) {
        //std::cout << "\tErased '" << i->first << "' by intersection." << endl;
        mffc.erase(i);
    }
}

void filterCurrentMffc(map<BnetNodeID, FFC>& ffc, const FFC& curr) {
    //std::cout << "No improvements, remove " << curr.name << "\n";
    ffc.erase(curr.name);
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