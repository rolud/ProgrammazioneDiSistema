//
// Created by rolud on 28/08/2019.
//

#include <QtWidgets/QGridLayout>
#include <iostream>
#include "DirectoryPieChart.h"
#include "DirEntry.h"

DirectoryPieChart::DirectoryPieChart(QWidget *parent): QWidget(parent) {

    fsm = FileSystemModel::getFileSystemModel();

    m_chart = new QtCharts::QChart();
    m_chart->setTitle(fsm->getPath());
    m_chart->setAnimationOptions(QtCharts::QChart::AllAnimations);


    m_series = new QtCharts::QPieSeries();
    std::vector<DirEntry> dirContent = fsm->getDirContent();
    for (auto iter = dirContent.begin(); iter != dirContent.end(); iter++) {
        m_series->append(iter->getName(), iter->getSize());
    }

    m_chart->addSeries(m_series);
    m_chart->legend()->hide();

    m_chartView = new QtCharts::QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(m_chartView, 0, 0);
    setLayout(baseLayout);

    connect(fsm, SIGNAL(pathChanged(const QString&)), this, SLOT(newPath(const QString&)));
    connect(m_series, SIGNAL(hovered(QPieSlice*, bool)), this, SLOT(showLabel(QPieSlice*, bool)));
}

//DirectoryPieChart::~DirectoryPieChart() {
//    delete m_series;
//    delete m_chartView;
//    delete m_chart;
//    delete fsm;
//}

void DirectoryPieChart::newPath(const QString &newpath) {
    if (fsm->getPath() == newpath) {
        m_chart->removeAllSeries();

        //delete m_series;
        m_series = nullptr;
        m_series = new QtCharts::QPieSeries();
        std::vector<DirEntry> dirContent = fsm->getDirContent();
        for (auto iter = dirContent.begin(); iter != dirContent.end(); iter++) {
            m_series->append(iter->getName(), iter->getSize());
        }

        m_chart->setTitle(fsm->getPath());
        m_chart->addSeries(m_series);

    }
}

void DirectoryPieChart::showLabel(QtCharts::QPieSlice *slice, bool state) {
    if (state) {
        std::cout << "Enter " << slice->label().toStdString() << " : " << slice->value() << std::endl;
    } else {
        std::cout << "Exit " << slice->label().toStdString() << " : " << slice->value() << std::endl;
    }
}