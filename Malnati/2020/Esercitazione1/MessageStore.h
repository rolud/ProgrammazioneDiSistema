//
// Created by rolud on 24/05/2020.
//

#ifndef ESERCITAZIONE1_MESSAGESTORE_H
#define ESERCITAZIONE1_MESSAGESTORE_H


#include <optional>
#include <tuple>

#include "Message.h"

class MessageStore {

    Message *messages;
    int dim; /** dimensione corrente */
    int n; /** incremento memoria */

public:
    MessageStore(int n);
    ~MessageStore();

    /**
     * inserisce nuovo messaggio o sovrascrive quello precedente
     * se ce n'è uno presente con lo stesso id
     * @param m
     */
    void add(Message m);

    /**
     * resituisce un messaggio se presente
     * @param id
     * @return
     */
    std::optional<Message> get(long id);

    /**
     * cancella un messaggio se presente
     * @param id
     * @return
     */
    bool remove(long id);

    /**
     * restituisce il numero di messaggi validi e di elementi vuoti
     * ancora disponibili
     * @return
     */
    std::tuple<int, int> stats();

    /**
     * compatta l'array (elimina le celle vuote e riporta l'array
     * alla dimensione multiplo di n minima in grade di contenere
     * tutte le celle
     */
     void compact();
};


#endif //ESERCITAZIONE1_MESSAGESTORE_H
