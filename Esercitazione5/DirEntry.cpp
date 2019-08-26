//
// Created by rolud on 24/08/2019.
//

#include <QDebug>
#include <iostream>

#include "DirEntry.h"

DirEntry::DirEntry(QFileInfo fileInfo) {
    this->_name = fileInfo.fileName();
    this->_parent = fileInfo.absolutePath();
    this->_size = fileInfo.size();
    this->_isDir = fileInfo.isDir();
    std::cout << this->_name.toStdString() << " : " << this->_size<< std::endl;
}

QString DirEntry::getName() {
    return _name;
}

qint64 DirEntry::getSize() {
    return _size;
}

bool DirEntry::isDir() {
    return _isDir;
}