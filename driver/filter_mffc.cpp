//
// Created by tripack on 4/16/17.
//

#include "header.h"

using std::map;
using std::vector;

typedef BlifBooleanNet::BnetNodeID BnetNodeID;
typedef BlifBooleanNet::FFC FFC;

void filterMffcBySize(map<BnetNodeID, FFC>& mffc) {
    auto iter= mffc.cbegin();
    while (iter != mffc.cend()) {
        if (iter->second.nodeSet.size() == 1) {
            std::cerr << "Erased '" << iter->first << "' by size." << std::endl;
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