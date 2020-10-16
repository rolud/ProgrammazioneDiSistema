#ifndef SQL_EXCEPTIONS_H
#define SQL_EXCEPTIONS_H

#include <QException>
#include <QString>
using namespace std;

/* parameter on create*/
class QueryException : public QException
{
    QString _msg;
public:
    QueryException(const QString& msg) : _msg(msg){}

    virtual const char* what() const noexcept override;
};

class ConnectionException : public QException
{
    QString _msg;
public:
    ConnectionException(const QString& msg) : _msg(msg){}

    virtual const char* what() const noexcept override;
};

class BadParamException : public QException
{
    QString _msg;
public:
    BadParamException(const QString& msg) : _msg(msg){}

    virtual const char* what() const noexcept override;
};

class InternalException : public QException
{
    QString _msg;
public:
    InternalException(const QString& msg) : _msg(msg){}

    virtual const char* what() const noexcept override;
};

#endif // SQL_EXCEPTIONS_H
