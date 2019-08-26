//
// Created by rolud on 24/08/2019.
//

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

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
        DirEntry de(iter->fileInfo());
        if (de.getName() == "." || de.getName() == "..") {
            continue;
        }

        this->dirContent.push_back(de);
    }

    delete iter;
    return this->dirContent;
}

void FileSystemModel::setPath(QString path) {
    if (this->path == path)
        return;
    this->path = std::move(path);
    emit pathChanged(this->path);
}