//
// Created by tripack on 16-12-16.
//

#ifndef VE490_BLIF_REPLACE_H
#define VE490_BLIF_REPLACE_H

#include <string>

void replacePartialBlif(const std::string& source,
                        const std::string& replaced,
                        const std::string& withFile,
                        const std::string& outFile);
#endif //VE490_BLIF_REPLACE_H
