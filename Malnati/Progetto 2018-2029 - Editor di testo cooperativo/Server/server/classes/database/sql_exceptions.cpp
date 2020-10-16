#include "include/sql_exceptions.h"

const char* QueryException::what() const noexcept
{
    return _msg.toStdString().c_str();
}

const char* ConnectionException::what() const noexcept
{
    return _msg.toStdString().c_str();
}

const char* BadParamException::what() const noexcept
{
    return _msg.toStdString().c_str();
}

const char* InternalException::what() const noexcept
{
    return _msg.toStdString().c_str();
}
