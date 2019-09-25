//
// Created by rolud on 25/09/2019.
//

#ifndef ESERCITAZIONE3_NETWORKSERVER_H
#define ESERCITAZIONE3_NETWORKSERVER_H


#include "SharedEditor.h"
#include "Message.h"

#include <queue>
#include <map>

class SharedEditor;

class NetworkServer {
public:
    NetworkServer();
    int connect(SharedEditor* sharedEditor);
    void disconnect(SharedEditor* sharedEditor);
    void send(const Message& msg);
    void dispatchMessages();
private:
    int _idcounter;
    std::map<int, SharedEditor*> _editors;
    std::queue<Message> _messages;

};


#endif //ESERCITAZIONE3_NETWORKSERVER_H
