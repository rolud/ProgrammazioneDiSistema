//
// Created by rolud on 24/08/2019.
//

#ifndef ESERCITAZIONE5_DIRENTRY_H
#define ESERCITAZIONE5_DIRENTRY_H


#include <QtCore/QString>
#include <QtCore/QFileInfo>

class DirEntry {
public:
    explicit DirEntry(QFileInfo fileInfo);
    QString getName();
    qint64 getSize();
    bool isDir();
    static qint64 dirSize(const QString& path);

private:
    QString _name;
    QString _parent;
    qint64 _size;
    bool _isDir;

};


#endif //ESERCITAZIONE5_DIRENTRY_H
