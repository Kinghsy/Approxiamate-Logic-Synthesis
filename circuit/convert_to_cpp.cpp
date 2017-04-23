//
// Created by tripack on 17-3-29.
//

#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>


#include "interface.h"

#include "../lib/libblif/interface.h"


using std::unordered_map;
using std::vector;
using std::string;
using std::set;

static unordered_map<string, string>
        createAlias(const set<string>& set, const string& setName) {
    unordered_map<string, string> alias;
    int counter = 0;
    for (auto& str: set) {
        assert(alias.count(str) == 0);
        alias[str] = setName + "[" + std::to_string(counter) + "]";
        counter++;
    }
    return alias;
}

static unordered_map<string, string>
createAlias(const vector<string>& set, const string& setName) {
    unordered_map<string, string> alias;
    int counter = 0;
    for (auto& str: set) {
        assert(alias.count(str) == 0);
        alias[str] = setName + "[" + std::to_string(counter) + "]";
        counter++;
    }
    return alias;
}

static string notG(const string& i) {
    return "(" + ("~" + i) + ")";
}

static string andG(const string& i1, const string& i2) {
    return "(" + i1 + " & " + i2 + ")";
}

static string xorG(const string& i1, const string& i2) {
    return "(" + i1 + " ^ " + i2 + ")";
}

static string getFunctionString(const string& i1,
                                const string& i2,
                                BnetNode* node) {
    std::bitset<4> truthTable(0);
    BnetTabline* f = node->f;
    //assert(f != nullptr);
    while (f != nullptr) {
        string line = f->values;
        assert(line.size() == 2);
        if (line == "00")
            truthTable[0] = 1;
        else if (line == "01")
            truthTable[1] = 1;
        else if (line == "10")
            truthTable[2] = 1;
        else if (line == "11")
            truthTable[3] = 1;
        else if (line == "-0")
            truthTable[0] = truthTable [2] = 1;
        else if (line == "0-")
            truthTable[0] = truthTable [1] = 1;
        else if (line == "-1")
            truthTable[1] = truthTable [3] = 1;
        else if (line == "1-")
            truthTable[2] = truthTable [3] = 1;
        else if (line == "--") {
            truthTable[0] = truthTable[1] = 1;
            truthTable[2] = truthTable[3] = 1;
        } else {
            std::cerr << "line = " << line << std::endl;
            assert(0);
        }
        f = f->next;
    }
    if (node->polarity == 1) truthTable.flip();
    switch (truthTable.to_ulong()) {
        case 0x0: return "false";
        case 0xF: return "true";
        case 0x1: return andG(notG(i1), notG(i2));
        case 0x2: return andG(notG(i1), i2);
        case 0x3: return notG(i1);
        case 0x4: return andG(i1, notG(i2));
        case 0x5: return notG(i2);
        case 0x6: return xorG(i1, i2);
        case 0x7: return notG(andG(i1, i2));
        case 0x8: return andG(i1, i2);
        case 0x9: return notG(xorG(i1, i2));
        case 0xA: return i2;
        case 0xB: return notG(andG(i1, notG(i2)));
        case 0xC: return i1;
        case 0xD: return notG(andG(notG(i1) , i2));
        case 0xE: return notG(andG(notG(i1), notG(i2)));
        default:
            assert(0);
    }
}

static string getFunctionString(const string& i,
                                BnetNode* node) {
    std::bitset<2> truthTable(0);
    BnetTabline* f = node->f;
    assert(f != nullptr);
    while (f != nullptr) {
        string line = f->values;
        assert(line.size() == 1);
        if (line == "0")
            truthTable[0] = 1;
        else if (line == "1")
            truthTable[1] = 1;
        else if (line == "-")
            truthTable[0] = truthTable [1] = 1;
        else {
            std::cerr << "line = " << line << std::endl;
            assert(0);
        }
        f = f->next;
    }
    if (node->polarity == 1) truthTable.flip();
    switch (truthTable.to_ulong()) {
        case 0x0: return "false";
        case 0x3: return "true";
        case 0x1: return i;
        case 0x2: return notG(i);
        default:
            assert(0);
    }
}

void BlifBooleanNet::exportToCpp(const std::string &fname) const {
    auto topSort = this->topologicalSort();
    auto inputs = this->inputNodeList();
    auto outputs = this->outputNodeList();
    auto internalSet = this->internalNodeSet();
    std::vector<string> internals(internalSet.begin(), internalSet.end());

    int nInputs = this->nInputs();
    int nOutputs = this->nOutputs();
    int nInternals = internals.size();

    std::ofstream ofile(fname);
    if (!ofile) {
        std::cerr << "Cannot open " << fname << std::endl;
        return;
    }

    unordered_map<string, string> inputAlias = createAlias(inputs, "input");
    unordered_map<string, string> outputAlias = createAlias(outputs, "output");
    unordered_map<string, string> internalAlias = createAlias(internals, "node");

    ofile << "// cpp built by tool\n";
    ofile << "#include <vector>\n";
    ofile << "#include <string>\n";

    ofile << "#define TYPE int\n";

    ofile << "extern \"C\" {\n"
          << "void circuit(const TYPE input[], TYPE output[], TYPE node[]);\n"
          << "std::vector<std::string> inputNode();\n"
          << "std::vector<std::string> outputNode();\n"
          << "std::vector<std::string> internalNode();\n"
          << "}\n";

    ofile << "void circuit(const TYPE input[], TYPE output[], TYPE node[])\n";
    ofile << "{\n";
    for (const auto& nodeName : inputs) {
        const auto& aliasName = inputAlias.at(nodeName);
        ofile << "\tconst TYPE& " << nodeName << " = " << aliasName << ";\n";
    }
    for (const auto& nodeName : outputs) {
        const auto& aliasName = outputAlias.at(nodeName);
        ofile << "\tTYPE& " << nodeName << " = " << aliasName << ";\n";
    }
    for (const auto& nodeName : internals) {
        const auto& aliasName = internalAlias.at(nodeName);
        ofile << "\tTYPE& " << nodeName << " = " << aliasName << ";\n";
    }
    for (const auto& node : topSort) {
        BnetNode* n = getNodeByName(node);
        if (n->type == BNET_INPUT_NODE) continue;
        if (n->type == BNET_CONSTANT_NODE) {
            if (n->polarity == 1) {
                ofile << "\t" <<  node << " = " << "false;\n";
            } else {
                ofile << "\t" << node << " = " << "true;\n";
            }
            continue;
        }
        if (n->ninp == 1) {
            std::string i = n->inputs[0];
            ofile << "\t" << node << " = " << getFunctionString(i, n) << ";\n";
            continue;
        }
        assert(n->ninp == 2);
        std::string i1 = n->inputs[0];
        std::string i2 = n->inputs[1];
        ofile << "\t" << node << " = " << getFunctionString(i1, i2, n) << ";\n";
    }

    // Normalize the result.
    ofile << "\tfor (int i = 0; i < " << nOutputs << "; ++i)\n";
    ofile << "\t\toutput[i] &= 0x01;\n";

    ofile << "\tfor (int i = 0; i < " << nInternals << "; ++i)\n";
    ofile << "\t\tnode[i] &= 0x01;\n";

//    for (const auto& node : topSort) {
//        if (inputs.count(node) != 0) continue;
//        BnetNode* n = getNodeByName(node);
//        assert(n->ninp == 2);
//        std::string i1 = n->inputs[0];
//        std::string i2 = n->inputs[1];
//        ofile << "bool " <<  node << " = " << getFunctionString(i1, i2, n) << ";\n";
//    }
    ofile << "}\n";

    // Prints the correspondence.

    ofile << "std::vector<std::string> inputNode() {\n";
    ofile << "return std::vector<std::string> {\n";
    int counter = 0;
    for (const string& name: inputs) {
        ofile << "\"" << name << "\", ";
        if (counter % 5 == 4) ofile << "\n";
        counter ++;
    }
    ofile << "\n};\n";
    ofile << "}\n";

    ofile << "std::vector<std::string> outputNode() {\n";
    ofile << "return std::vector<std::string> {\n";
    counter = 0;
    for (const string& name: outputs) {
        ofile << "\"" << name << "\", ";
        if (counter % 5 == 4) ofile << "\n";
        counter ++;
    }
    ofile << "\n};\n";
    ofile << "}\n";

    ofile << "std::vector<std::string> internalNode() {\n";
    ofile << "return std::vector<std::string> {\n";
    counter = 0;
    for (const string& name: internals) {
        ofile << "\"" << name << "\", ";
        if (counter % 5 == 4) ofile << "\n";
        counter ++;
    }
    ofile << "\n};\n";
    ofile << "}\n";

    ofile.close();
}


