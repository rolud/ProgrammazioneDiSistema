//
// Created by rolud on 25/09/2019.
//

#include <string>
#include "SharedEditor.h"
#include "NetworkServer.h"

SharedEditor::SharedEditor(NetworkServer &server) : _server(server) {
    _siteId = _server.connect(this);
    _counter = 100;
}


void SharedEditor::localInsert(int index, char value) {
    _counter++;
    if (!_symbols.empty()) {
        if (index )
        Symbol next = _symbols.at(index);
        Symbol prev = _symbols.at(index - 1);

    } else {
        std::vector<int> pos;
        pos.push_back(index);
        std::string id = std::to_string(_siteId) + std::to_string(_counter);
        Symbol msg(id, value, pos);
    }
}

void SharedEditor::localErase(int index) {

}

void SharedEditor::process(const Message &msg) {

}

std::string SharedEditor::to_string() {
    return nullptr;
}

int SharedEditor::get_siteId() const {
    return _siteId;
}

const std::vector<Symbol> &SharedEditor::get_symbols() const {
    return _symbols;
}
