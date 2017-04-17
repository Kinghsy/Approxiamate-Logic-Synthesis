//
// Created by tripack on 17-4-12.
//

#include <set>
#include "blif_builder.h"
#include "ttable.h"
#include <algorithm>
#include <sstream>
#include <fstream>


BlifBuilder::BlifBuilder(const NodeName &node, bool flip) {
    input.insert(std::make_pair(node, flip));
    outputNodeName = node;
}

BlifBuilder
combineBilfBuilder(const BlifBuilder &d1, const BlifBuilder &d2,
                   const TTable &table, const NodeName &newOutput) {
    if (table == ALL_IRR_TABLE_0)
        return BlifBuilder::BuildConst(newOutput, false);
    if (table == ALL_IRR_TABLE_1)
        return BlifBuilder::BuildConst(newOutput, true);

    if (table == LEFT_RELA_TABLE) {
        auto ret = d1;
        if (ret.isConst()) {
            ret.outputNodeName = newOutput;
            return ret;
        }
        auto it = ret.input.find(ret.outputNodeName);
        if (it != ret.input.end()) {
            auto value = it->second;
            ret.input.erase(it);
            ret.input.emplace(newOutput, value);
            ret.outputNodeName = newOutput;
            return ret;
        }
        assert(!ret.data.empty());
        auto& last = *ret.data.rbegin();
        assert(last.out == ret.outputNodeName);

        ret.outputNodeName = newOutput;
        last.out = newOutput;

        return ret;
    }

    if (table == RIGHT_RELA_TABLE) {
        auto ret = d2;
        if (ret.isConst()) {
            ret.outputNodeName = newOutput;
            return ret;
        }
        auto it = ret.input.find(ret.outputNodeName);
        if (it != ret.input.end()) {
            auto value = it->second;
            ret.input.erase(it);
            ret.input.emplace(newOutput, value);
            ret.outputNodeName = newOutput;
            return ret;
        }
        assert(!ret.data.empty());
        auto& last = *ret.data.rbegin();
        assert(last.out == ret.outputNodeName);

        ret.outputNodeName = newOutput;
        last.out = newOutput;

        return ret;
    }

    BlifBuilder ret;
    ret.outputNodeName = newOutput;

    BlifBuilder::Connection connection (
            newOutput,
            d1.outputNodeName,
            d2.outputNodeName,
            table
    );

    ret.data.insert(ret.data.end(), d1.data.begin(), d1.data.end());
    ret.data.insert(ret.data.end(), d2.data.begin(), d2.data.end());

    ret.data.push_back(connection);

    ret.input.insert(d1.input.begin(), d1.input.end());
    ret.input.insert(d2.input.begin(), d2.input.end());

    ret.constant.insert(d1.constant.begin(), d1.constant.end());
    ret.constant.insert(d2.constant.begin(), d2.constant.end());

    return ret;
}

NodeName BlifBuilder::outputNode() const {
    return outputNodeName;
}

void BlifBuilder::exportBlif(const std::string &filename) const {
    std::ofstream ofs(filename);
    if (!ofs) assert(0);
    ofs << *this;
}

std::set<NodeName> BlifBuilder::inputName() const {
    std::set<NodeName> ret;
    for (const auto& elem : input) {
        ret.insert(elem.first);
    }
    return ret;
}


BlifBuilder::Connection::Connection
        (const NodeName &out_,
         const NodeName &i1_, const NodeName &i2_,
         const TTable &method_)
        : out(out_), in1(i1_), in2(i2_), method(method_) {}

std::ostream &operator<<(std::ostream &ss,
                         const BlifBuilder::Connection &connection) {
    ss << ".names " << connection.in1 << " " << connection.in2 << " ";
    ss << connection.out << "\n";
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
            os << elem.first << " ";
        }
    }
    os << "\n";
    os << ".outputs " << builder.outputNodeName << "\n";

    return builder.printBody(os) << ".end\n";
}

std::ostream &BlifBuilder::printBody(std::ostream &os) const {
    for (const auto& pair : constant) {
        if (pair.second) {
            os << ".names " << pair.first << "\n";
            os << "1" << "\n\n";
        } else {
            os << ".names " << pair.first << "\n\n";
        }
    }

    for (const auto& con : this->data) {
        auto connection = con;
        bool flip1 = input.count(con.in1) && input.at(con.in1);
        bool flip2 = input.count(con.in2) && input.at(con.in2);
        flipBinaryTTableInput(connection.method, flip1, flip2);
        os << connection;
    }

    return os;
}

BlifBuilder BlifBuilder::BuildConst(const NodeName &node, bool value) {
    BlifBuilder builder;
    builder.constant.emplace(node, value);
    builder.outputNodeName = node;
    return builder;
}
