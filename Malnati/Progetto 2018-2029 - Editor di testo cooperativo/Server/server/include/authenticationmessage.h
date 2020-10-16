#ifndef AUTHENTICATIONMESSAGE_H
#define AUTHENTICATIONMESSAGE_H
#include "include/message.h"
#include <QJsonObject>
#include <QString>

class AuthenticationMessage : public Message
{
    public:
        enum EventType{ LOGIN, LOGOUT, REGISTER, UPDATE, SERVER_RESPONSE };

        AuthenticationMessage( const int clientId, const QString& userName, const EventType eventType, const QString& password, const int icon, const bool operationSuccess, const QString& serverMessage );

        EventType getEventType() const;
        QString getUserName() const;
        QString getPassword() const;
        int getIcon() const;
        bool isOperationSuccess() const;
        QString getServerMessage() const;

        static AuthenticationMessage fromJsonObject( const QJsonObject& jsonObject );


    protected:
        void toJsonObject( QJsonObject& jsonObject ) const override;


    private:
        QString userName;
        QString password;
        int icon;
        EventType eventType;

        // Attributi per il messaggio SERVER_RESPONSE
        // servono per comunicare al client se l'operazione di login o register
        // ha avuto successo o se è fallita; se è fallita l'errore è specificato
        // in serverMessage
        bool operationSuccess;
        QString serverMessage;
};

#endif // AUTHENTICATIONMESSAGE_H
