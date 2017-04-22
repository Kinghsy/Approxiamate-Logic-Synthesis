//
// Created by tripack on 17-4-12.
//

#include <set>
#include "blif_builder.h"
#include "ttable.h"
#include <algorithm>
#include <sstream>
#include <fstream>

BlifBuilder BlifBuilder::buildConst(const NodeName &node, bool value) {
    if (value) return BlifBuilder(CONST_1, node);
    return BlifBuilder(CONST_0, node);
}

BlifBuilder BlifBuilder::buildInput(const NodeName &node, bool value) {
    if (value) return BlifBuilder(REVERSE_INPUT, node);
    return BlifBuilder(INPUT, node);
}

BlifBuilder
combineBilfBuilder(const BlifBuilder &d1, const BlifBuilder &d2,
                   const TTable &table, const NodeName &newOutput) {

    if (table == ALL_IRR_TABLE_0)
        return BlifBuilder::buildConst(newOutput, false);
    if (table == ALL_IRR_TABLE_1)
        return BlifBuilder::buildConst(newOutput, true);

    if (table == LEFT_RELA_TABLE) {
        auto ret = d1;
        *ret.node = newOutput;
        return ret;
    }

    if (table == RIGHT_RELA_TABLE) {
        auto ret = d2;
        *ret.node = newOutput;
        return ret;
    }


    BlifBuilder ret(BlifBuilder::TYPE::NET, newOutput);

    BlifBuilder::Connection connection;
    connection.out = ret.node; // Must be the same pointer
    connection.in1 = d1.node;
    connection.in2 = d2.node;
    connection.method = table;


    bool rev1 = d1.type == BlifBuilder::TYPE::REVERSE_INPUT;
    bool rev2 = d2.type == BlifBuilder::TYPE::REVERSE_INPUT;

    flipBinaryTTableInput(connection.method, rev1, rev2);

    ret.data.insert(ret.data.end(), d1.data.begin(), d1.data.end());
    ret.data.insert(ret.data.end(), d2.data.begin(), d2.data.end());

    ret.data.push_back(connection);

    ret.input.insert(ret.input.end(), d1.input.begin(), d1.input.end());
    ret.input.insert(ret.input.end(), d2.input.begin(), d2.input.end());

    ret.constant0.insert(ret.constant0.end(), d1.constant0.begin(), d1.constant0.end());
    ret.constant0.insert(ret.constant0.end(), d2.constant0.begin(), d2.constant0.end());

    ret.constant1.insert(ret.constant1.end(), d1.constant1.begin(), d1.constant1.end());
    ret.constant1.insert(ret.constant1.end(), d2.constant1.begin(), d2.constant1.end());

    return ret;
}

NodeName BlifBuilder::outputNode() const {
    return *node;
}

void BlifBuilder::exportBlif(const std::string &filename) const {
    std::ofstream ofs(filename);
    if (!ofs) assert(0);
    ofs << *this;
}

std::set<NodeName> BlifBuilder::inputName() const {
    std::set<NodeName> ret;
    for (const auto& elem : input)
        ret.insert(*elem);
    return ret;
}


std::ostream &operator<<(std::ostream &ss,
                         const BlifBuilder::Connection &connection) {
    assert(connection.method.nInputs() == 2);
    ss << ".names " << *connection.in1 << " " << *connection.in2 << " ";
    ss << *connection.out << "\n";
    ss << "00 " << connection.method[0] << "\n";
    ss << "10 " << connection.method[1] << "\n";
    ss << "01 " << connection.method[2] << "\n";
    ss << "11 " << connection.method[3] << "\n";
    ss << std::endl;
    return ss;
}

std::ostream &operator<<(std::ostream &os, const BlifBuilder &builder) {
    os << ".model bignode\n";
    if (!builder.input.empty()) {
        os << ".inputs ";
        for (const auto &elem : builder.input) {
            os << *elem << " ";
        }
    }
    os << "\n";
    os << ".outputs " << *builder.node << "\n";

    return builder.printBody(os) << ".end\n";
}

std::ostream &BlifBuilder::printBody(std::ostream &os) const {
    for (const auto& elem : constant1) {
        os << ".names " << *elem << "\n";
        os << "1" << "\n\n";
    }
    for (const auto& elem : constant0) {
        os << ".names " << *elem << "\n\n";
    }

    for (const auto& con : this->data) {
        auto connection = con;
        os << connection;
    }

    return os;
}

