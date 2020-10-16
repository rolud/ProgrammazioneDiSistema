#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

    signals:
        void tryAgain();

    private slots:
        void on_TryAgain_Button_clicked();

        void on_OKButton_2_clicked();

    private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
