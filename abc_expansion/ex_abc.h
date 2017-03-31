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



private:
    static ExAbc* instance;
    void* abcFramework = nullptr;

    ExAbc();
    ExAbc(const ExAbc& exAbc) {}
    ExAbc(ExAbc&& exAbc) noexcept {}
    ExAbc& operator=(const ExAbc& exAbc) {return *this;}
    ExAbc& operator=(ExAbc&& exAbc) noexcept {return *this;}
    ~ExAbc() noexcept;

};


#endif //VE490_EX_ABC_H
