#ifndef ICONSELECTION_H
#define ICONSELECTION_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class IconSelection;
}

class IconSelection : public QDialog
{
    Q_OBJECT

public:
    explicit IconSelection(QWidget *parent = nullptr);   
    ~IconSelection();

signals:

    void iconSelected(int icon);

private slots:
    void on_IconList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::IconSelection *ui;
    int icon;


};

#endif // ICONSELECTION_H
