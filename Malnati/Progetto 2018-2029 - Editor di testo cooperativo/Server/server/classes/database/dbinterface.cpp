#include "include/dbinterface.h"
#include <exception>

DBinterface::DBinterface()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    query = QSqlQuery("", db);
    //v1
    //QString path = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0];
    //path.append("shared_docs.db");
    //db.setDatabaseName(path);
    //v2
    //db.setDatabaseName("C:/Users/Giovanni/shared_docs_test.db");
    //v3
    QString db_path = QDir::currentPath();
    db_path =  db_path + QString("/../server/database/shared_docs.db");
    db.setDatabaseName(db_path);

}

void DBinterface::openDBConnection()
{
    if(!db.open()){
        throw ConnectionException("failed to open db!");
    }else{
        qDebug() << "db opened";
    }
}

void DBinterface::closeDBConnection()
{
    db.close();
}


uint DBinterface::createUser(QString receivedUsername, QString receivedPassword, uint avatarNumber) //return userId
{
    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }

    if(receivedPassword=="") {
        throw BadParamException("Password not valid.");
    }
    query.prepare("SELECT * FROM user WHERE username = :username");
    query.bindValue(":username", receivedUsername);
    if(query.exec()) {
        if(!query.next()) {
            query.prepare("INSERT INTO user (uid, username, password, avatar) VALUES (NULL, :username, :password, :avatar)");
            query.bindValue(":username", receivedUsername);
            query.bindValue(":password", receivedPassword);
            query.bindValue(":avatar", avatarNumber);
            if(query.exec()) {
                qDebug("User correctly created");
                return query.lastInsertId().toUInt();
            }
            else{
                //throw  QueryException(query.lastError().databaseText());
                throw QueryException("Error inserting new user");
            }
        }
        else {
            throw BadParamException("A user with the selected username already exists");
        }
    }
    else {
        //throw  QueryException(query.lastError().databaseText());
        throw QueryException("Error selecting existing users");
    }
}





void DBinterface::checkCredentials(QString receivedUsername, QString receivedPassword)
{
    if(!db.isOpen()){
        throw ConnectionException("db not opened");
    }
    query.prepare("SELECT * FROM user WHERE username = :username AND password = :password");
    query.bindValue(":username", receivedUsername);
    query.bindValue(":password", receivedPassword);
    if(query.exec()) {
        if(!query.next()) {
            throw BadParamException("invalid username or password");
        }
    }
    else{
        throw  QueryException("Error selecting existing users in database");
    }
}


int DBinterface::getUID(QString receivedUsername) {
    int uid = -1;
    if(!db.isOpen()) {
        return uid;
    }
    query.prepare("SELECT uid FROM user WHERE username = :username");
    query.bindValue(":username", receivedUsername);
    if(query.exec()) {
        if(query.next())
            uid = query.value(0).toInt();
    }
    else {
        qDebug("Failed to exec query for UID");
    }
    return uid;
}


int DBinterface::getDBIcon(uint userId) {
    int icon = -1;
    if(!db.isOpen()) {
        return icon;
    }
    query.prepare("SELECT avatar FROM user WHERE uid = :userid");
    query.bindValue(":userid", userId);
    if(query.exec()) {
        if(query.next()) {
            icon = query.value(0).toInt();
        }
    }
    else {
        qDebug("Failed to exec icon query");
    }
    return icon;
}


void DBinterface::updateUserProfile(QString receivedUsername, uint iconNumber)
{
    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }
    query.prepare("UPDATE user SET avatar = :avatar WHERE username = :username");
    query.bindValue(":avatar", iconNumber);
    query.bindValue(":username", receivedUsername);
    if(query.exec()) {
        if(query.numRowsAffected() == 1)
            qDebug("Icon correctly updated");
        else
            throw BadParamException("Invalid username");
    }
    else
        throw QueryException(query.lastError().databaseText());
}

void DBinterface::updateUserPassword(QString receivedUsername, QString receivedPassword)
{
    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }
    query.prepare("UPDATE user SET password = :password WHERE username = :username");
    query.bindValue(":username", receivedUsername);
    query.bindValue(":password", receivedPassword);
    if(query.exec()) {
        if(query.numRowsAffected() == 1)
            qDebug("Password correctly updated");
        else
            throw BadParamException("Invalid username");
    }
    else
        throw QueryException(query.lastError().databaseText());
}


void DBinterface::updateAllProfile(uint userId, QString receivedUsername, QString receivedPassword, uint iconNumber)
{
    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }
    query.prepare("UPDATE user SET avatar = :avatar, username = :username, password = :password WHERE uid = :userid");
    query.bindValue(":avatar", iconNumber);
    query.bindValue(":password", receivedPassword);
    query.bindValue(":username", receivedUsername);
    query.bindValue(":userid", userId);
    if(query.exec()) {
        if(query.numRowsAffected() == 1)
            qDebug("Profile correctly updated");
        else
            throw BadParamException("Invalid uid received");
    }
    else{
        //throw QueryException(query.lastError().databaseText()); //ritorna la stringa UNIQUE failed:user.username
        throw QueryException("Error: username already used!");
    }
}


void DBinterface::addDocumentAccess(QString receivedUsername, QString docName)
{
    int uid;

    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }
    uid = getUID(receivedUsername);
    if(uid < 0) {
        throw InternalException("Invalid username");
    }


    query.prepare("SELECT did FROM has_access h WHERE h.did = :docname AND h.uid = :username");
    query.bindValue(":docname", docName);
    query.bindValue(":username", uid);
    if(query.exec()) {
        if(!query.next()) {
            qDebug("has_access NOT contains doc");
            query.prepare("INSERT OR IGNORE INTO has_access (uid, did) VALUES (:uid, :did)");
            query.bindValue(":uid", uid);
            query.bindValue(":did", docName);
            if(query.exec())
                qDebug("Access correctly granted");
            else
                throw QueryException(query.lastError().databaseText());
        }
        else {
            qDebug("has_access contains already the doc");
        }

    }else
        throw QueryException(QueryException(query.lastError().databaseText()));


}


QList<QString> DBinterface::getAccessibileDocuments(QString receivedUsername)
{
    QList<QString> documentNames;
    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }

    query.prepare("SELECT did FROM has_access h "
                  "WHERE h.uid = (SELECT uid FROM user WHERE username = :username)");
    query.bindValue(":username", receivedUsername);
    if(query.exec()) {
        while(query.next()) {
            documentNames.push_back(query.value(0).toString());
        }
    }
    else
        throw QueryException(query.lastError().databaseText());

    return documentNames;
}



bool DBinterface::insertDocument(QString receivedDoc) {

    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }

    query.prepare("SELECT docname FROM documents WHERE docname = :received");
    query.bindValue(":received", receivedDoc);
    if(query.exec()) {
        if(!query.next()) {
            query.prepare("INSERT INTO documents (docname) VALUES (:received)");
            query.bindValue(":received", receivedDoc);
            if(query.exec()) {
                qDebug("Document added");
                return true;
            }
            else{
                throw std::invalid_argument("Error inserting new document.");
            }
        }
        else {
            throw std::invalid_argument("A document with the same name already exists.");
        }
    }else {
        throw std::invalid_argument("Error selecting existing documents.");
    }

}



bool DBinterface::getDocument(QString receivedDoc) {
    if(!db.isOpen()) {
        throw ConnectionException("DB is not opened");
    }

    query.prepare("SELECT docname FROM documents WHERE docname = :received");
    query.bindValue(":received", receivedDoc);
    if(query.exec()) {
        if(query.next()) {
            qDebug("document present.");
            return true;
        }
        else {
            throw std::invalid_argument("The url inserted does not correspond to an existing document.");
        }
    }else {
        throw std::invalid_argument("Error selecting existing documents");
    }
}
