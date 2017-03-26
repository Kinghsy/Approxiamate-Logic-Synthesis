//
// Created by tripack on 17-3-26.
//

#include "resource.h"

File::File(const std::string &base,
           const std::string &ext)
        : base(base), ext(ext) {}

File File::operator[] (const std::string &name) const {
    return File(base + "_" + name, ext);
}

File::operator std::string() const {
    return this->toString();
}

std::string File::toString() const {
    return base + ext;
}

File File::operator[](int i) const {
    return this->operator[](std::to_string(i));
}

FileClass::FileClass(const std::string &ext)
        : ext(ext) {
    if (ext == "" || ext.front() == '.') return;
    this->ext = "." + ext;
}

File FileClass::operator()(const std::string &name) const {
    return File(name, ext);
}

std::string FileClass::toString() const {
    return "FILENAME" + ext;
}

Path::Path(const std::string &p)
        : path(p) {
    while (!path.empty() && path.back() == '/' ) {
        path.pop_back();
    }
    path += "/";
}

std::string Path::toString() const {
    return path;
}

std::string Path::operator/(const std::string &f) const {
    return path + f;
}

Path Path::sub(const std::string &f) const {
    std::string s = f;
    while (!s.empty() && s.front() == '/') {
        s.erase(s.begin());
    }
    if (s.empty()) return *this;
    return Path(this->path + s );
}

Path ProjectBase(PROJECT_BASE);
Path McncPath = ProjectBase.sub("bin/run");
Path McncAigPath = ProjectBase.sub("bin/run");
FileClass fBlif("blif");
