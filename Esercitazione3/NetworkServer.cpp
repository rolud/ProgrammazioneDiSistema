//
// Created by rolud on 25/09/2019.
//

#include "NetworkServer.h"
#include <map>

NetworkServer::NetworkServer() {
    idcounter_ = 100;
}

int NetworkServer::connect(SharedEditor *sharedEditor) {
    if (sharedEditor == nullptr) return -1;
    idcounter_++;
    editors_.insert(std::pair<int,SharedEditor*>(idcounter_, sharedEditor));
    return idcounter_;
}

void NetworkServer::disconnect(SharedEditor *sharedEditor) {
    if (sharedEditor == nullptr) return;
    editors_.erase(sharedEditor->get_siteId());
}

void NetworkServer::send(const Message &msg) {
    messages_.push(msg);
}

void NetworkServer::dispatchMessages() {
    while (!messages_.empty()) {
        Message msg = messages_.front();
        for (auto iter = editors_.begin(); iter != editors_.end(); iter++) {
            if (iter->first != msg.get_editor_id())
                iter->second->process(msg);
        }
        messages_.pop();
    }
}
