//
// Created by rolud on 25/06/2020.
//
/**
 * E' presente una classe Dispatcher<T> che gestisce sottoscrizioni, inserendole tramite un metodo
 * "subscribe(void(*f)(T t))" e le cancella tramite il metodo "unsubscribe(void(*f)(T t))". Esiste un ulteriore metodo chiamato
 * "notify(T t)" che deve permettere al thread corrente di eseguire le sottoscrizioni, tramite l'invocazione della "subscribe()"
 * oppure rimuoverle tramite la "unsubscribe()".
 * Qualora in presenza di una notifica dovesse arrivare un'altra notifica dal thread corrente, questa verrà accodata, mentre
 * se la notifica arriva da un altro thread questa verrà bloccata. Realizzare in C++11 la classe con i relativi metodi.
 */
#ifndef EXAMS_DISPATCHER_H
#define EXAMS_DISPATCHER_H

template <class T>
class Dispatcher {
public:

    void subscribe(void(*f)(T t)) {

    }

    void subscribe() {

    }

    void unsubscribe(void(*f)(T t)) {

    }

    void unsubscribe() {

    }

    void notify(T t) {

    }

private:

};

#endif //EXAMS_DISPATCHER_H
