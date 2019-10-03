//
// Created by rolud on 25/09/2019.
//

#include <string>
#include <cmath>
#include <random>
#include "SharedEditor.h"
#include "NetworkServer.h"
#include "Message.h"

SharedEditor::SharedEditor(NetworkServer &server) : _server(server) {
    _siteId = _server.connect(this);
    _counter = 100;
    _boundery = 10;
    _base = 32;
}

/**
 * inserisce il simbolo nel vettore visualizzato in locale
 * crea e invia al server il messaggio con l'azione di inserimento
 *
 * @param index
 * @param value
 */
void SharedEditor::localInsert(int index, char value) {
    const Symbol sym = genereteSymbol(index, value);
    if (index == _symbols.size()) {
        _symbols.push_back(sym);
    }
    _symbols.insert(_symbols.begin() + index, sym);
    Message msg(sym, Message::Action::INSERT);
    _server.send(msg);
}

/**
 * crea il simbolo e aumenta il contatore per l'identificativo
 *
 * @param index
 * @param value
 * @return
 */
Symbol SharedEditor::genereteSymbol(int index, char value) {
    const std::vector<int> posBefore = _symbols[index - 1].get_position();
    const std::vector<int> posAfter  = _symbols[index].get_position();
    const std::vector<int> newPos    = generatePosBetween(posBefore, posAfter);
    _counter++;
    Symbol newSym(std::to_string(_siteId) + std::to_string(_counter), value, newPos);
    return newSym;
}

/**
 * funzione ricorsiva per ricercare in quale posizione inserire il nuovo simbolo
 * analizza le due posizioni tra le quali il simbolo deve essere inserito, se c'è spazio tra le due posizioni
 * inserisce subito, altrimenti scende di livello nell'albero
 *
 * @param posBefore
 * @param posAfter
 * @param newPos
 * @param level
 * @return il nuovo vettore posizione
 */
std::vector<int> SharedEditor::generatePosBetween(std::vector<int> posBefore, std::vector<int> posAfter, std::vector<int> newPos, int level) {
    int pos1, pos2;
    int base = static_cast<int>(pow(2, level) * _base);
    std::string boundary_strategy = choiceBoundaryStrategy(level);

    if (level <= posBefore.size()) pos1 = posBefore[level];
    else pos1 = 0;

    if (!posAfter.empty()) pos2 = posAfter[0];
    else pos2 = base;

    if (pos2 - pos1 > 1) {
        // we can insert
        int pos = generatePos(pos1, pos2, boundary_strategy);
        newPos.push_back(pos);
        return newPos;
    } else if (pos2 - pos1 == 0) {
        // explore next level
        newPos.push_back(pos1);
        return generatePosBetween(posBefore, {}, newPos, level + 1);
    } // todo handle pos2==pos1
}

/**
 * genera casualmente la posizione in base alla strategia:
 * boundary+ : posizione casuale vicina al limite inferiore
 * boundary- : posizione casuale vicina al limite superiore
 *
 * @param min : limite inferiore
 * @param max : limite superiore
 * @param boundaryStrategy : + per boundary+, - per boundary-
 * @return posizione selezionata
 */
int SharedEditor::generatePos(int min, int max, std::string boundaryStrategy) {
    if ((max - min) < _boundery) {
        min = min + 1;
    } else {
        if (boundaryStrategy == "+") {
            min = max - _boundery;
        } else {
            min = min + 1;
            max = min + _boundery;
        }
    }
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<int> dist{min, max};
    return dist(e);
}

/**
 * determina la strategia di confinamento
 * al momento alterna boundary+ e boundary- con la profondità del livello
 * è possibile implementare altre strategie
 *
 * @param level
 * @return
 */
std::string SharedEditor::choiceBoundaryStrategy(int level) {
    return (level % 2) == 0 ? "+" : "-"; // todo we can implement other strategy
}

/**
 * elimina il symbolo dal vettore locale
 * crea e invia al server il messaggio di eliminazione
 *
 * @param index
 */
void SharedEditor::localErase(int index) {
    Symbol sym = _symbols[index];
    _symbols.erase(_symbols.begin() + index);
    Message msg(sym, Message::Action::DELETE);
    _server.send(msg);
}

/**
 * ricevuto il messaggio dal server, compie l'azione contenuta
 *
 * @param msg
 */
void SharedEditor::process(const Message &msg) {

}

/**
 * converte il vettore di simboli in stringa
 *
 * @return
 */
std::string SharedEditor::to_string() {
    return nullptr;
}

int SharedEditor::get_siteId() const {
    return _siteId;
}

const std::vector<Symbol> &SharedEditor::get_symbols() const {
    return _symbols;
}


