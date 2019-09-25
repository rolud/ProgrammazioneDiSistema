//
// Created by rolud on 25/09/2019.
//

#include "NetworkServer.h"
#include <map>

NetworkServer::NetworkServer() {
    _idcounter = 100;
}

int NetworkServer::connect(SharedEditor *sharedEditor) {
    if (sharedEditor == nullptr) return -1;
    _editors.insert(std::pair<int,SharedEditor*>(_idcounter, sharedEditor));
    _idcounter++;
    return _idcounter;
}

void NetworkServer::disconnect(SharedEditor *sharedEditor) {
    if (sharedEditor == nullptr) return;
    _editors.erase(sharedEditor->get_siteId());
}

void NetworkServer::send(const Message &msg) {
    _messages.push(msg);
}

void NetworkServer::dispatchMessages() {
    while (!_messages.empty()) {
        Message m = _messages.front();
        // send message to all but not to creator
        _messages.pop();
    }
}
