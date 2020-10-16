#include <QApplication>
#include <QObject>

#include "include/controller.h"


int main( int argv, char* args[] )
{
    QApplication a{ argv, args };

    Controller controller{};
    controller.showStartupDialog();

    return a.exec();
}
