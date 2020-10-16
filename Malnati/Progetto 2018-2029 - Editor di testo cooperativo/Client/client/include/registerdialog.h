#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

public slots:
    void on_RegisterPushButton_clicked();
    void on_SelectIconPushButton_clicked();
    void onUserRegistered();
    void showIcon (int icon);
    void setBoxTitle ( const QString& title );
    void clear();
    void setUserData( const QString& username, const QString& password, const int icon );

signals:

    void openLoginDialog();
    void loginRequest( const QString& username, const QString& password );
    void RegisterRequest( const QString& username, const QString& password, int icon );
    void openIconSelection();
    void incompleteData( const QString& errorMessage );


private slots:

    void on_RegisterPushButton_2_clicked();

private:
    Ui::RegisterDialog *ui;


    QString username, password, iconPath,buttonTitle;

    int icon=-1;




};

#endif // REGISTERDIALOG_H
