//
// Created by tripack on 17-3-27.
//

#include <resource.h>
#include <boost/filesystem.hpp>
#include <iostream>

#include "../../abc_expansion/ex_abc.h"
#include "../../circuit/interface.h"

using namespace boost::filesystem;

int main() {

    const Path& orig = BenchmarkPath;
    const Path& _aig = BenchmarkAigPath;

    path original(orig.toString());
    path aig(_aig.toString());

    if (!is_directory(original)) {
        std::cout << McncPath.toString() << " is not path." << std::endl;
        return -1;
    }
    if (!is_directory(aig)) {
        std::cout << McncPath.toString() << " is not path." << std::endl;
        return -1;
    }

    ExAbc& abc = ExAbc::getInstance();

    for (directory_entry& e : directory_iterator(original))  {
        if (e.path().extension() == ".blif") {
//            abc.loadBlif(e.path().string());
//            abc.toAig();
//            abc.writeBlif(McncAigPath / e.path().filename().string());
//            abc.empty();
            std::cout << "Reading " << e.path().filename() << std::endl;
            abc.loadBlif(e.path().string());
            abc.resyn2();
            abc.writeBlif(_aig / e.path().filename().string());
            abc.empty();
//            abc.loadBlif(_aig / e.path().filename().string());
//            abc.empty();
        }
    }

    for (directory_entry& e : directory_iterator(original)) {
        if (e.path().extension() == ".blif") {
            std::string name = e.path().filename().string();
            std::cout << name << " : ";
            abc.loadBlif(orig / name);
            std::cout << abc.nGates() << " / " ;
            abc.loadBlif(_aig / name);
            std::cout << abc.nGates() << " | ";
//            if (name == "") {
//                std::cout << std::endl;
//                abc.empty();
//                continue;
//            }
            // std::cout << "Reading " << orig / name << std::endl;
            BlifBooleanNet netOrig(orig / name);
            // std::cout << "Reading " << _aig / name << std::endl;
            BlifBooleanNet netAig(_aig / name);
            std::cout << netOrig.gateCount()
                      << " / " << netAig.gateCount();
            std::cout << std::endl;
            abc.empty();
        }
    }


    return 0;
}
