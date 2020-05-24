//
// Created by rolud on 07/08/2019.
//

#ifndef ESERCITAZIONE2_DIRECTORY_H
#define ESERCITAZIONE2_DIRECTORY_H

#include <list>
#include <string>
#include <memory>
#include "Base.h"
#include "File.h"

class Directory : public Base {
public:
    static std::shared_ptr<Directory> getRoot();
    std::shared_ptr<Directory> addDirectory(std::string name);
    std::shared_ptr<File> addFile(std::string nome, uintmax_t size);
    std::shared_ptr<Base> get(std::string name);
    std::shared_ptr<Directory> getDir(std::string name);
    std::shared_ptr<File> getFile(std::string name);
    void remove(std::string name);
    void ls(int indent = 0) const override;
    int mType() const override;

    Directory(const Directory&) = delete;
    void operator= (const Directory&) = delete;

protected:
    Directory(std::string name);

private:
    static std::shared_ptr<Directory> instance;
    std::list<std::shared_ptr<Base>> files;
    std::weak_ptr<Directory> parent;
    std::weak_ptr<Directory> self;

};


#endif //ESERCITAZIONE2_DIRECTORY_H
