#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <fstream>
#include "sql_exceptions.h"

class DBinterface
{
public:
    explicit DBinterface();
    void openDBConnection();
    void closeDBConnection();
    uint createUser(QString, QString, uint);
    void checkCredentials(QString, QString);
    int getUID(QString );
    int getDBIcon(uint);
    void updateUserProfile(QString , uint);
    void updateUserPassword(QString , QString );
    void updateAllProfile(uint,QString, QString , uint);
    QList<QString> getAccessibileDocuments(QString);
    void addDocumentAccess(QString, QString);
    bool insertDocument(QString);
    bool getDocument(QString);

private:
    QSqlDatabase db;
    QSqlQuery query;


};

#endif // DBINTERFACE_H
