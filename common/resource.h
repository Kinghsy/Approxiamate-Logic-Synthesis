//
// Created by tripack on 17-3-26.
//

#ifndef VE490_RESOURCE_H
#define VE490_RESOURCE_H

#include <string>

class File {
    std::string base;
    std::string ext;
public:
    File(const std::string& base, const std::string& ext);
    File operator[] (const std::string &name) const;
    File operator[] (int i) const;
    operator std::string() const;
    std::string toString() const;
};

class FileClass {
    std::string ext;
public:
    FileClass(const std::string& ext);
    File operator()(const std::string& name) const;
    std::string toString() const;
};

class Path {
    std::string path;
public:
    Path(const std::string& p);
    std::string toString() const;
    std::string operator/(const std::string& f) const;
    Path sub(const std::string& f) const;
};

extern Path ProjectBase;
extern Path McncPath;
extern Path McncAigPath;
extern FileClass fBlif;


#endif //VE490_RESOURCE_H
