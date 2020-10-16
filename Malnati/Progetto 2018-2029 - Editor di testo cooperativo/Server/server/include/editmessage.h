#ifndef EDITMESSAGE_H
#define EDITMESSAGE_H


#include "include/message.h"
#include "include/symbol.h"
#include <QJsonObject>

class EditMessage : public Message
{

    public:
        EditMessage( const int clientId, const int pos );

        int getPosition() const;
        int getClientId() const;
        void setClientId(int id);
        static EditMessage fromJsonObject( const QJsonObject& jsonObject );
protected:
        void toJsonObject( QJsonObject& jsonObject ) const override;
private:
        int position;
        int clientId;
};


#endif // EDITMESSAGE_H
