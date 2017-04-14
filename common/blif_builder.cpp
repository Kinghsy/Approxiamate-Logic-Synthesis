//
// Created by tripack on 17-4-12.
//

#include <set>
#include "blif_builder.h"
#include "ttable.h"
#include <algorithm>
#include <sstream>



BlifBuilder::BlifBuilder(const NodeName &node, bool flip) {
    input.insert(std::make_pair(node, flip));
    outputNodeName = node;
}

BlifBuilder
combineBilfBuilder(const BlifBuilder &d1, const BlifBuilder &d2,
                   const TTable &table, const NodeName &newOutput) {
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

    return ret;
}

NodeName BlifBuilder::outputNode() const {
    return outputNodeName;
}

void BlifBuilder::exportBlif(const std::string &filename) const {

}

std::set<NodeName> BlifBuilder::inputName() const {
    std::set<NodeName> ret;
    for (const auto& elem : input) {
        ret.insert(elem.first);
    }
    return ret;
}

std::string BlifBuilder::Connection::toBlifString() const {
    std::stringstream ss;
    ss << ".name " << this->in1 << " " << this->in2 << " ";
    ss << this->out << "\n";
    ss << "00 " << this->method[0] << "\n";
    ss << "01 " << this->method[1] << "\n";
    ss << "10 " << this->method[2] << "\n";
    ss << "11 " << this->method[3] << "\n";
    ss << std::endl;

    return ss.str();
}

BlifBuilder::Connection::Connection
        (const NodeName &out_,
         const NodeName &i1_, const NodeName &i2_,
         const TTable &method_)
        : out(out_), in1(i1_), in2(i2_), method(method_) {}
