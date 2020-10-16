#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include "include/message.h"


class ErrorMessage : public Message
{
    public:
        ErrorMessage( const int clientId, const QString& errorMessage );

        QString getErrorMessage() const;

        static ErrorMessage fromJsonObject( const QJsonObject& jsonObject );

    protected:
        void toJsonObject( QJsonObject& jsonObject ) const override;

    private:
        QString errorMessage;
};

#endif // ERRORMESSAGE_H
