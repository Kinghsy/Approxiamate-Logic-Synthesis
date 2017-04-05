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

static const Path ProjectBase(PROJECT_BASE);
static const Path DataPath = ProjectBase.sub("data");

static const Path CircuitPath = DataPath.sub("circuit");

static const Path BenchmarkPath = CircuitPath.sub("other_original");
static const Path BenchmarkAigPath = CircuitPath.sub("other_fraig");

static const Path McncPath = CircuitPath.sub("mcnc_original");
static const Path McncAigPath = CircuitPath.sub("mcnc_aig");
static const Path McncFraigPath = CircuitPath.sub("mcnc_fraig");

static const Path PrecompDbPath = DataPath.sub("precomp");

static const Path TempPath = ProjectBase.sub("bin/temp");

static const FileClass fBlif("blif");
static const FileClass fCpp("cpp");
static const FileClass fSo("so");

#endif //VE490_RESOURCE_H
