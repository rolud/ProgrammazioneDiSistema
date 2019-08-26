//
// Created by rolud on 24/08/2019.
//

#ifndef ESERCITAZIONE5_FILESYSTEMMODEL_H
#define ESERCITAZIONE5_FILESYSTEMMODEL_H


#include <QtCore/QObject>
#include "DirEntry.h"

class FileSystemModel: public QObject {
    Q_OBJECT

public:
    static FileSystemModel* getFileSystemModel();
    QString getPath();
    std::vector<DirEntry> getDirContent();
    void setPath(QString path);

    FileSystemModel(const FileSystemModel& fsm) = delete;
    FileSystemModel(FileSystemModel&& fsm) = delete;

signals:
    void pathChanged(const QString& path);

protected:
    FileSystemModel();
private:
    static FileSystemModel* fsm;
    QString path;
    std::vector<DirEntry> dirContent;




};


#endif //ESERCITAZIONE5_FILESYSTEMMODEL_H
