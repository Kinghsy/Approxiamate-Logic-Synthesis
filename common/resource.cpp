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
    if (path == "") {
        path = "/";
        return;
    }
    if (path.back() == '/') return;
    path += "/";
}

std::string Path::toString() const {
    return path;
}

std::string Path::operator/(const std::string &f) const {
    return path + f;
}

Path McncPath("");
Path McncAigPath("");
FileClass fBlif("blif");
