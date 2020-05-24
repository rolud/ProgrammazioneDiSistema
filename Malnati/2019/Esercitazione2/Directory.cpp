//
// Created by rolud on 07/08/2019.
//

#include <iostream>
#include "Directory.h"
#include "Base.h"

std::shared_ptr<Directory> Directory::instance = nullptr;

Directory::Directory(std::string name) {
    this->setName(name);
}

std::shared_ptr<Directory> Directory::getRoot() {
    if (Directory::instance == nullptr) {
        Directory::instance = std::shared_ptr<Directory>(new Directory("/"));
        Directory::instance->parent = Directory::instance;
        Directory::instance->self = Directory::instance;
    }
    return instance;
}

std::shared_ptr<Directory> Directory::addDirectory(std::string name) {
    for (auto const &f : this->files) {
        if (f->getName() == name)
            throw std::exception();
    }

    std::shared_ptr<Directory> child = std::shared_ptr<Directory>(new Directory(name));
    child->parent = this->self;
    child->self = self;

    this->files.push_back(child);

    return child;
}

std::shared_ptr<File> Directory::addFile(std::string nome, uintmax_t size) {
    for (auto const &f : this->files) {
        if (f->getName() == nome)
            throw std::exception();
    }

    std::shared_ptr<File> file = std::make_shared<File>(nome,size);

    this->files.push_back(file);

    return file;
}

std::shared_ptr<Base> Directory::get(std::string name) {
    if (name == ".")
        return this->self.lock();

    if (name == "..")
        return this->parent.lock();

    for (auto const &f : this->files) {
        if (name == f->getName())
            return f;
    }

    return std::shared_ptr<Base>();
}

std::shared_ptr<Directory> Directory::getDir(std::string name) {
    if (name == ".")
        return this->self.lock();

    if (name == "..")
        return this->parent.lock();

    for (auto const &f : this->files) {
        if (name == f->getName() && f->mType() == 1)
            return std::dynamic_pointer_cast<Directory>(f);
    }

    return std::shared_ptr<Directory>();
}

std::shared_ptr<File> Directory::getFile(std::string name) {
    for (auto const &f : this->files) {
        if (name == f->getName() && f->mType() == 0)
            return std::dynamic_pointer_cast<File>(f);
    }

    return std::shared_ptr<File>();
}

void Directory::remove(std::string name) {
    if (name == "." || name == "..") throw std::exception();
    for (auto const &f : this->files) {
        if (name == f->getName()) {
            this->files.remove(f);
            break;
        }
    }
}

void Directory::ls(int indent) const {
    for (int i = 0; i < indent; i++)
        std::cout << "\t";
    std::cout << "[+] " << this->getName() << std::endl;
    auto iter = this->files.begin();
    auto end = this->files.end();
    for (; iter != end; iter++) {
        (*iter)->ls(indent + 1);
    }
}
int Directory::mType() const {return 1;}