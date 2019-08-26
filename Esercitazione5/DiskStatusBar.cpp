//
// Created by rolud on 23/08/2019.
//

#include "DiskStatusBar.h"

#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtWidgets/QGridLayout>
#include <QtCore/QStorageInfo>

#include <QDebug>

DiskStatusBar::DiskStatusBar(QWidget *parent): QWidget(parent) {
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle("Dischi");
    chart->setAnimationOptions(QtCharts::QChart::AllAnimations);

    m_series = new QtCharts::QHorizontalStackedBarSeries();

    QStringList categories = setDiskInfo();

    chart->addSeries(m_series);

    QtCharts::QBarCategoryAxis *axisY = new QtCharts::QBarCategoryAxis();
    axisY->append(categories);
    chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisY);

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    m_series->attachAxis(axisX);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView = new QtCharts::QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(m_chartView, 0, 0);
    setLayout(baseLayout);
}

QStringList DiskStatusBar::setDiskInfo() {
    QtCharts::QBarSet *set_availablespace = new QtCharts::QBarSet("Spazio libero");
    QtCharts::QBarSet *set_occupiedspace = new QtCharts::QBarSet("Spazio Utilizzato");
    QStringList categories;

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!storage.isReadOnly()) {
//                qDebug() << "name:" << storage.name();
//                qDebug() << "fileSystemType:" << storage.fileSystemType();
//                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";

                long percentuale = storage.bytesAvailable() * 100 / storage.bytesTotal();
                QString catname = storage.name() + " (" + QString::number(storage.bytesTotal()/1000/1000/1000) + " GB)";
                categories << catname;
                set_availablespace->append(percentuale);
                set_occupiedspace->append(100 - percentuale);
            }
        }
    }

    m_series->append(set_availablespace);
    m_series->append(set_occupiedspace);

    return categories;
}