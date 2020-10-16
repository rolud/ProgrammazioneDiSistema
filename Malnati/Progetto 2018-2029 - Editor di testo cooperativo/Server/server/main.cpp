#include <QCoreApplication>
#include "include/server.h"
#include <QFile>

QFile logFile{ "log.txt" };

// Ridireziona tutti i qDebug() sul file log.txt
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    ( void )type;
    ( void )context;

    logFile.write( ( msg.toStdString() + "\n" ).c_str() );
    logFile.flush();
}


int main(int argc, char *argv[])
{    
    // Decommentando si ridireziona tutti i messaggi qDebug su file invece che sulla console
    // logFile.open( QIODevice::WriteOnly );
    // qInstallMessageHandler( messageHandler );
    qRegisterMetaType< Client* >( "Client*" );

    QCoreApplication a(argc, argv);

    Server server{};
    server.start();

    return a.exec();
}
