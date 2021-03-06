//
// Created by tripack on 17-3-25.
//

#ifndef VE490_EX_ABC_H
#define VE490_EX_ABC_H

#include <string>
#include <map>
#include <set>

class ExAbc {
public:
    static ExAbc& getInstance();
    int execute(const std::string& str);
    int loadBlif(const std::string& file);
    int writeBlif(const std::string& file);
    int toAig();
    std::map<std::string, std::set<std::string> > Mffc();
    int nGates();
    int empty();
    void resyn2();
    void rewrite(bool z = false);
    void balance();
    void refactor(bool z = false);
    void map();
    void reboot();
    double postMapArea();



private:
    static ExAbc* instance;
    void* abcFramework = nullptr;

    ExAbc();
    ExAbc(const ExAbc& exAbc) = delete;
    ExAbc(ExAbc&& exAbc) noexcept = delete;
    ExAbc& operator=(const ExAbc& exAbc) = delete;
    ExAbc& operator=(ExAbc&& exAbc) = delete;
    ~ExAbc() noexcept;

};


#endif //VE490_EX_ABC_H
