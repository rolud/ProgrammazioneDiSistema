#include <iostream>

#include <QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGridLayout>
#include <QtCharts>
#include "DiskStatusBar.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow mainWindow;
    QWidget *mainWidget = new QWidget(&mainWindow);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    mainWindow.setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);

    DiskStatusBar *disks = new DiskStatusBar(mainWidget);

    mainLayout->addWidget(disks);

    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}