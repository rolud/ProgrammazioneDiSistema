//
// Created by rolud on 28/08/2019.
//

#ifndef ESERCITAZIONE5_DIRECTORYPIECHART_H
#define ESERCITAZIONE5_DIRECTORYPIECHART_H


#include <QtWidgets/QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include "FileSystemModel.h"

class DirectoryPieChart: public QWidget {
    Q_OBJECT
public:
    explicit DirectoryPieChart(QWidget* parent = nullptr);
    // ~DirectoryPieChart();

public slots:
    void newPath(const QString& newpath);
    void showLabel(QtCharts::QPieSlice* slice, bool state);

private:
    QtCharts::QPieSeries* m_series;
    QtCharts::QChartView* m_chartView;
    QtCharts::QChart* m_chart;
    FileSystemModel* fsm;


};


#endif //ESERCITAZIONE5_DIRECTORYPIECHART_H
