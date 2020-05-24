//
// Created by rolud on 24/08/2019.
//

#ifndef ESERCITAZIONE5_SEARCHBAR_H
#define ESERCITAZIONE5_SEARCHBAR_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

class SearchBar : public QWidget {
    Q_OBJECT

public:
    SearchBar(QWidget *parent = nullptr);

public slots:
    void selected(const QString& entry);
    void selected();
private:
    QLineEdit* m_lineEdit;
    QPushButton* m_btnUpDir;
    QCompleter* m_completer;
    QLayout* m_layout;
};


#endif //ESERCITAZIONE5_SEARCHBAR_H
