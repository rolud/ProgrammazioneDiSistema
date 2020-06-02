//
// Created by rolud on 02/06/2020.
//

#ifndef ESERCITAZIONE2_DIRECTORY_H
#define ESERCITAZIONE2_DIRECTORY_H

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "Base.h"
#include "File.h"

class Directory : public Base {

    std::string m_name;
    std::weak_ptr<Directory> m_self;
    std::weak_ptr<Directory> m_parent;
    std::map<std::string, std::shared_ptr<Base>> m_children;

    static std::shared_ptr<Directory> root;

    explicit Directory(std::string name);
public:

    ~Directory() = default;

    static std::shared_ptr<Directory> getRoot();
    static std::shared_ptr<Directory> makeDirectory(std::string name, std::weak_ptr<Directory> parent);

    std::shared_ptr<Directory> addDirectory(std::string name);
    std::shared_ptr<File> addFile(std::string name, uintmax_t size);


    Base::Type mType() const override;
    void ls(int indent) const override;

    std::weak_ptr<Directory> getParent() const;
    std::map<std::string, std::shared_ptr<Base>> getChildren() const;

    std::shared_ptr<Base> get(std::string name) const;
    std::shared_ptr<Directory> getDir(const std::string& name) const;
    std::shared_ptr<File> getFile(const std::string& name) const;

    bool remove(const std::string& name);


};




#endif //ESERCITAZIONE2_DIRECTORY_H
