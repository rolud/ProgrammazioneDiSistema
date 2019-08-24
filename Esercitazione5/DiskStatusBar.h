//
// Created by rolud on 23/08/2019.
//

#ifndef ESERCITAZIONE5_DISKSTATUSBAR_H
#define ESERCITAZIONE5_DISKSTATUSBAR_H


#include <QtWidgets/QWidget>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QChartView>

class DiskStatusBar: public QWidget {
public:
    DiskStatusBar(QWidget *parent = 0);
private:
    QtCharts::QHorizontalStackedBarSeries *m_series;
    QtCharts::QChartView *m_chartView;

    QStringList setDiskInfo();
};


#endif //ESERCITAZIONE5_DISKSTATUSBAR_H
