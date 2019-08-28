//
// Created by rolud on 24/08/2019.
//

#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <iostream>

#include "FileSystemModel.h"
#include "DirEntry.h"

FileSystemModel* FileSystemModel::fsm = nullptr;

FileSystemModel::FileSystemModel(): QObject() {
    path = QDir::currentPath();
}

FileSystemModel* FileSystemModel::getFileSystemModel() {
    if (!fsm)
        fsm = new FileSystemModel();
    return fsm;
}

QString FileSystemModel::getPath() {
    return path;
}

std::vector<DirEntry> FileSystemModel::getDirContent() {
    auto *iter = new QDirIterator(path);

    while(iter->hasNext()) {
        iter->next();
        if (iter->fileName() == "." || iter->fileName() == "..") {
            continue;
        }
        DirEntry de(iter->fileInfo());
        std::cout << "File: " << de.getName().toStdString() << " - Size: " << de.getSize() << std::endl;
        this->dirContent.push_back(de);
    }

    delete iter;
    return this->dirContent;
}

void FileSystemModel::setPath(QString path) {
    if (this->path == path)
        return;
    this->path = std::move(path);
    this->dirContent.clear();
    emit pathChanged(this->path);
}