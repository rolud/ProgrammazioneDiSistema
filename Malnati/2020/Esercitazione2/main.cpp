#include <iostream>
#include <memory>
#include "Directory.h"

int main() {

    auto root = Directory::getRoot();
    auto first = root->addDirectory("first");
    auto second = root->addDirectory("second");
    first->addDirectory("A")->addDirectory("I");
    first->addDirectory("B")->addDirectory("II")->addFile("pds.zip", 31520);
    first->addFile("main.c", 32);
    first->addFile("Base.h", 71);
    second->addFile("image.jpg", 1024);
    auto c = second->addDirectory("C");
    c->addDirectory("III")->addDirectory("x");
    c->addDirectory("IV")->addDirectory("y")->addFile("beta.zip", 500);
    c->getDir("III")->getDir("x")->addFile("alpha.tar", 200);
    root->ls(0);
    std::cout << " => removing directory second .. " << std::endl;
    root->remove("second");
    root->ls(0);
    return 0;
}