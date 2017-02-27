//
// Created by tripack on 17-2-27.
//

#ifndef VE490_PROFILE_H
#define VE490_PROFILE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>


class CircuitProfile {
    std::string circuitName;

    struct Count {
        int trueCount = 0;
        int total = 0;

        void incTrue() {total++; trueCount++;}
        void incFalse() {total++;}
    };

    std::unordered_map<std::string, Count> data;

public:
    // Parameter is the name of the circuited under profile
    CircuitProfile(const std::string& name);

    // Declare one node is true(false) in one round of test
    // Allows to take in STL sets to set multiple at once
    void incTrue(const std::string& nodeName);
    void incTrue(const std::unordered_set<std::string>& nodeName);
    void incTrue(const std::set<std::string>& nodeName);

    void incFalse(const std::string& nodeName);
    void incFalse(const std::unordered_set<std::string>& nodeName);
    void incFalse(const std::set<std::string>& nodeName);

    // Return # of TRUEs divided by # of total samples
    double getProbability(const std::string& nodeName) const;

    // Return total number of test performed on a node
    int getTotal(const std::string& nodeName) const;

    // Return number of TRUEs of a node
    int getTrueCount(const std::string& nodeName) const;

    // Return number of FALSEs of a node
    int getFalseCount(const std::string& nodeName) const;

    // Return if a node has been profiled.
    // i.e. return if profile data for a node exists
    bool isProfiled(const std::string& nodeName) const;

    // Combines 2 profiles.
    // Essentially for each node adds up the total and trueCount
    // P.S. for now I don't need a separate operator+ just += is enough
    CircuitProfile& operator+=(const CircuitProfile& p2);
};



#endif //VE490_PROFILE_H
