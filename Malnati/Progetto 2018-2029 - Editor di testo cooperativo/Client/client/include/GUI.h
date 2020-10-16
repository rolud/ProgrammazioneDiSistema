#ifndef GUI_H
#define GUI_H


#include <QObject>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QEvent>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>




#include <string>


class GUI  : public QObject
{
Q_OBJECT
    public:
        explicit GUI( QObject* parent = nullptr );

        GUI( const GUI& gui ) = delete; // Elimina il costruttore di copia
        GUI& operator=( const GUI& gui ) = delete; // Elimina l'operatore di assegnazione


        void startGUI();

        // Il clientId viene fornito dal server al momento della connessione


        // Connette il client al server all'indirizzo e porta specificati negli attributi privati.


    protected:
        // Serve per leggere ogni 25ms dal buffer di tastiera senza essere bloccante.
        // Quando poi ci sarà la GUI questo metodo sarà inutile in quanto si gestirà
        // tutto con gli eventi QKeyPress.


    private:



    signals:

    private slots:
        // Slots per intercettare i segnali del socket


};
















#endif // GUI_H
