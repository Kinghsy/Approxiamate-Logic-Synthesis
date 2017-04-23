//
// Created by tripack on 17-3-29.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>

#include "../circuit/interface.h"
#include "../abc_expansion/ex_abc.h"
#include "../newApprox/src/decomp_small.h"
#include "header.h"


int main() {

    PreDecomp::getInstance();

    StopWatch sw;
    auto circuit = fBlif("C880");
    sw.start();
    auto net = BlifBooleanNet(BenchmarkAigPath / circuit);
    sw.take("Load");
    auto mffcs = net.getFFC();
    sw.take("Mffc");
    filterMffcBySize(mffcs);
    sw.take("FilterSize");
    filterMffcContainOutput(mffcs, net.outputNodeList());
    sw.take("FilterOutput");

//    for (const auto& pair : mffcs) {
//        cout << std::setw(10) << pair.first << " ";
//        const auto& attr = pair.second;
//        cout << attr.inputNode.size() << " " << attr.nodeSet.size() << " ";
//        for (auto &&item : attr.nodeSet) {
//            cout << item << " ";
//            assert(!net.outputNodeSet().count(item) || item == pair.first);
//        }
//        cout << endl;
//    }

    net.prepareSimulator();

    sw.take("Compiling");

    auto simResult = net.profileBySimulation(10000);

    sw.take("Simulation");


    auto test =
            [](const FFC& ffc) -> bool {
                return ((ffc.inputNode.size() <= 6) &&
                        (ffc.inputNode.size() >= 4));
            };

    auto ffc = findFirstFFC(mffcs, test);
    sw.take("FindFFC");

    while (ffc) {
        std::cout << "Found MFFC of " << ffc->name << " over "
                  << ffc->inputNode.size() << " inputs, "
                  << ffc->nodeSet.size() << " nodes\n";
        net.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
        auto mffcBlif = BlifBooleanNet(TempPath / fBlif("mffc"));
        std::cout << "Inputs: " << mffcBlif.inputNodeList() << std::endl;
        std::cout << "Ouputs: " << mffcBlif.outputNodeList() << std::endl;
        sw.take("FFC Load");

        BoolFunction mffcFunction(
                mffcBlif.nInputs(),
                mffcBlif.truthTable(),
                mffcBlif.inputNodeList(),
                mffcBlif.outputNodeList()[0]);

        auto match = AlgorithmDecomposeSmall(0.005f)(mffcFunction, simResult);

        if (match.deInfo.nNode() == ffc->nodeSet.size()) {
            filterCurrentMffc(mffcs, *ffc);
        } else {
            //std::cout << blifBuilder;
            sw.take("BuildBlif");

            match.deInfo.exportBlif(TempPath / fBlif("mffc_approx"));
            auto approxNet = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
            sw.take("ReloadBlif");

            filterMffcByIntersection(mffcs, *ffc);
        }
        sw.take("Filter");
        ffc = findFirstFFC(mffcs, test);
        sw.take("FindFFC");
    }

    return 0;
}