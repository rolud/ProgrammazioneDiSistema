//
// Created by rolud on 24/08/2019.
//

#include <QCompleter>
#include <QtWidgets/QDirModel>
#include <QtWidgets/QHBoxLayout>
#include <iostream>

#include "SearchBar.h"
#include "FileSystemModel.h"

SearchBar::SearchBar(QWidget *parent) : QWidget(parent) {
    // FileSystemModel* fsm = FileSystemModel::getFileSystemModel();
    QString path = FileSystemModel::getFileSystemModel()->getPath();

    m_lineEdit = new QLineEdit(path, this);
    // m_lineEdit->setPlaceholderText(path);
    // m_lineEdit->setFocus();

    m_completer = new QCompleter(this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setModel(new QDirModel(m_completer));

    m_lineEdit->setCompleter(m_completer);

    connect(m_completer, SIGNAL(activated(const QString&)), this, SLOT(selected(const QString&)));
    connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(selected()));

    m_btnUpDir = new QPushButton("UP", this);

    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_lineEdit);
    m_layout->addWidget(m_btnUpDir);
    this->setLayout(m_layout);
}

void SearchBar::selected(const QString& entry) {
    FileSystemModel::getFileSystemModel()->setPath(entry);
    FileSystemModel::getFileSystemModel()->getDirContent();
// std::cout << "Line Edit -> " << entry.toStdString() << std::endl;
}

void SearchBar::selected() {
    FileSystemModel::getFileSystemModel()->setPath(m_lineEdit->text());
    // std::cout << "Line Edit -> " << m_lineEdit->text().toStdString() << std::endl;
}
