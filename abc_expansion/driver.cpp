//
// Created by tripack on 17-3-26.
//

#include "ex_abc.h"
#include <iostream>
#include <resource.h>

int main() {
    Path p = ProjectBase.sub("testblif");
    std::cout << p.toString() << std::endl;

    ExAbc& exAbc = ExAbc::getInstance();
    exAbc.loadBlif(p / fBlif("c880"));
    //exAbc.execute("print_mffc");
    //std::map<std::string, std::set<std::string> > m = exAbc.Mffc();
//    for (const auto& pair : m) {
//        std::cout << pair.first << std::endl << "\t" ;
//        for (const auto& str : pair.second) {
//            std::cout <<  str << " ";
//        }
//        std::cout << std::endl;
//    }
    std::cout << exAbc.nGates() << std::endl;
    return 0;
}