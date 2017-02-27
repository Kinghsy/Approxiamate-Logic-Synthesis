//
// Created by tripack on 17-2-27.
//

#include <cassert>
#include "profile.h"


CircuitProfile::CircuitProfile(const std::string &name) {
    this->circuitName = name;
}

void CircuitProfile::incTrue(const std::string &nodeName) {
    this->data[nodeName].incTrue();
}

void CircuitProfile::incFalse(const std::string &nodeName) {
    this->data[nodeName].incFalse();
}

double CircuitProfile::getProbability(const std::string &nodeName) const {
    double trueCount = getTrueCount(nodeName);
    double totalCount = getTotal(nodeName);
    return trueCount / totalCount;
}

int CircuitProfile::getTotal(const std::string &nodeName) const {
    return data.at(nodeName).total;
}

int CircuitProfile::getTrueCount(const std::string &nodeName) const {
    return data.at(nodeName).trueCount;
}

int CircuitProfile::getFalseCount(const std::string &nodeName) const {
    const CircuitProfile::Count& count = data.at(nodeName);
    return count.total - count.trueCount;
}

bool CircuitProfile::isProfiled(const std::string &nodeName) const {
    return data.count(nodeName) > 0;
}

CircuitProfile &CircuitProfile::operator+=(const CircuitProfile &p2) {
    if (p2.circuitName != circuitName) assert(0);
    for (const auto& pair : p2.data) {
        auto& count = data[pair.first];
        auto& count2 = p2.data.at(pair.first); // Must always exists
        count.trueCount += count2.trueCount;
        count.total += count2.total;
    }
    return *this;
}

void CircuitProfile::incTrue(const std::unordered_set<std::string> &nodeName) {
    for (const auto& name : nodeName) incTrue(name);
}

void CircuitProfile::incTrue(const std::set<std::string> &nodeName) {
    for (const auto& name : nodeName) incTrue(name);
}

void CircuitProfile::incFalse(const std::unordered_set<std::string> &nodeName) {
    for (const auto& name : nodeName) incFalse(name);

}

void CircuitProfile::incFalse(const std::set<std::string> &nodeName) {
    for (const auto& name : nodeName) incFalse(name);
}
