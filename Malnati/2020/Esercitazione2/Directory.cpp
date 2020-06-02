//
// Created by rolud on 02/06/2020.
//

#include "Directory.h"

std::shared_ptr<Directory> Directory::root = nullptr;

Directory::Directory(std::string name) {
    Base::m_name = name;
}
//
//Directory::~Directory() {
//    std::cout << "~Directory() @ " << this->m_name << std::endl;
//}

std::weak_ptr<Directory> Directory::getParent() const { return this->m_parent; }

std::map<std::string, std::shared_ptr<Base>> Directory::getChildren() const { return this->m_children; }

std::shared_ptr<Base> Directory::get(std::string name) const {
    if (name == ".") return this->m_self.lock();
    else if (name == "..") return this->m_parent.lock();

    auto child = this->m_children.find(name);
    if (child == this->m_children.end()) return std::shared_ptr<Base>();
    return child->second;
}

std::shared_ptr<Directory> Directory::getDir(const std::string &name) const {
    if (name == ".") return this->m_self.lock();
    else if (name == "..") return this->m_parent.lock();
    auto child = this->m_children.find(name);
    if (child == this->m_children.end() || child->second->mType() == Base::FILE)
        return std::shared_ptr<Directory>();

    return std::dynamic_pointer_cast<Directory>(child->second);
}

std::shared_ptr<File> Directory::getFile(const std::string &name) const {
    if (name == "." || name == "..") return std::shared_ptr<File>();

    auto child = this->m_children.find(name);
    if (child == this->m_children.end() || child->second->mType() == Base::DIRECTORY)
        return std::shared_ptr<File>();

    return std::dynamic_pointer_cast<File>(child->second);
}


std::shared_ptr<Directory> Directory::addDirectory(std::string name) {
    std::shared_ptr<Directory> d = makeDirectory(name, this->m_self);
    this->m_children.insert(std::make_pair(name, d));
    return d;
}

std::shared_ptr<File> Directory::addFile(std::string name, uintmax_t size) {
    auto file = std::make_shared<File>(name, size);
    this->m_children.insert(std::make_pair(name, file));
    return file;
}

bool Directory::remove(const std::string &name) {
    if (name == "." || name == "..") return false;
    auto child = this->m_children.find(name);
    if (child == this->m_children.end()) return false;
    this->m_children.erase(name);
    return true;
}


// INHERITED FROM BASE

Base::Type Directory::mType() const { return Base::DIRECTORY; }

void Directory::ls(int indent) const {
    std::cout << std::string(indent * 4, ' ') << "[+] " << Base::m_name << std::endl;
    std::for_each(this->m_children.begin(), this->m_children.end(), [&indent](auto& child){ child.second->ls(indent + 1); });
}

// STATIC FUNCTIONS

std::shared_ptr<Directory> Directory::getRoot() {
    if (root == nullptr) {
        root = makeDirectory("/", std::shared_ptr<Directory>(nullptr));
    }
    return root;
}

std::shared_ptr<Directory> Directory::makeDirectory(std::string name, std::weak_ptr<Directory> parent) {
    auto *d = new Directory(name);
    std::shared_ptr<Directory> d_ptr = std::shared_ptr<Directory>(d);
    d->m_self = d_ptr;
    d->m_parent = parent;
    return d_ptr;
}