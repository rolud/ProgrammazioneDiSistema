//
// Created by rolud on 24/08/2019.
//

#include <QDebug>
#include <iostream>
#include <QtCore/QDirIterator>

#include "DirEntry.h"

DirEntry::DirEntry(QFileInfo fileInfo) {
    this->_name = fileInfo.fileName();
    this->_parent = fileInfo.absolutePath();
    this->_size = dirSize(_parent + "/" + _name);
    this->_isDir = fileInfo.isDir();
    // std::cout << this->_parent.toStdString() << "/" << this->_name.toStdString() << " : " << this->_size<< std::endl;
    // std::cout << "                 Size -> " << dirSize(_parent + "/" + _name ) << std::endl;
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

qint64 DirEntry::dirSize(const QString& path) {
    qint64 size = 0;
    // std::cout << " ---- " << path.toStdString() << std::endl;
    auto file = new QFileInfo(path);
    if (!file->isDir())
        size = file->size();
    else {
        auto *iter = new QDirIterator(path);
        while (iter->hasNext()) {
            iter->next();
            if (iter->fileName() == "." || iter->fileName() == "..") {
                continue;
            }
            if (iter->fileInfo().isDir()) {
                // std::cout << " ==== " << iter->filePath().toStdString()  << std::endl;
                size = size + dirSize(iter->filePath());
            } else {
                size = size + iter->fileInfo().size();
            }
        }
        delete iter;
    }
    delete file;
    return size;
}