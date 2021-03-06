#ifndef USERMESSAGE_H
#define USERMESSAGE_H

#include "include/message.h"
#include <QJsonObject>
#include <QString>


class UserMessage : public Message
{
    public:
        enum EventType{ CONNECTED, DISCONNECTED };

        UserMessage( const int clientId, const QString& userName, const EventType eventType, const int icon );

        EventType getEventType() const;
        QString getUserName() const;
        int getIcon() const;

        static UserMessage fromJsonObject( const QJsonObject& jsonObject );


    protected:
        void toJsonObject( QJsonObject& jsonObject ) const override;


    private:
        QString userName;
        EventType eventType;
        int icon;
};

#endif // USERMESSAGE_H
