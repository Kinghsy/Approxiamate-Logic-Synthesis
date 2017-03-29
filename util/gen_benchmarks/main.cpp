//
// Created by tripack on 17-3-27.
//

#include <resource.h>
#include <boost/filesystem.hpp>
#include <iostream>

#include "../../abc_expansion/ex_abc.h"
#include "../../MFFC/interface.h"

using namespace boost::filesystem;

int main() {


    path mcncBPath(McncPath.toString());
    if (!is_directory(mcncBPath)) {
        std::cout << McncPath.toString() << " is not path." << std::endl;
        return -1;
    }

    ExAbc& abc = ExAbc::getInstance();

//    for (directory_entry& e : directory_iterator(mcncBPath))  {
//        if (e.path().extension() == ".blif") {
//            abc.loadBlif(e.path().string());
//            abc.toAig();
//            abc.writeBlif(McncAigPath / e.path().filename().string());
//            abc.empty();
//            abc.loadBlif(e.path().string());
//            abc.execute("fraig");
//            abc.writeBlif(McncFraigPath / e.path().filename().string());
//            abc.empty();
//        }
//    }

    for (directory_entry& e : directory_iterator(path(mcncBPath))) {
        if (e.path().extension() == ".blif") {
            std::string name = e.path().filename().string();
            std::cout << name << " : ";
            abc.loadBlif(McncAigPath / name);
            std::cout << abc.nGates() << " / " ;
            abc.loadBlif(McncFraigPath / name);
            std::cout << abc.nGates() << " | ";
            if (name == "s38584.1.blif" ||
                    name == "diffeq.blif" ||
                    name == "s298.blif" ||
                    name == "tseng.blif" ||
                    name == "bigkey.blif" ||
                    name == "elliptic.blif" ||
                    name == "clma.blif" ||
                    name == "dsip.blif" ||
                    name == "s38417.blif" ||
                    name == "frisc.blif") {
                std::cout << std::endl;
                abc.empty();
                continue;
            }
            BlifBooleanNet netAig(McncAigPath / name);
            BlifBooleanNet netFraig(McncFraigPath / name);
            std::cout << netAig.gateCount() << " / " <<  netFraig.gateCount() << std::endl;
            abc.empty();
        }
    }


    return 0;
}
