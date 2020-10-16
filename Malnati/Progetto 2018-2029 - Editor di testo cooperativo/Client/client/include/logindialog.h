#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "include/registerdialog.h"
#include "include/client.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

public slots:

    void on_LoginPushButton_clicked();

    void on_RegisterPushButton_clicked();

    void onUserAuthenticated();

    void clear();

signals:
    void openRegisterDialog(const QString& title);
    void loginRequest( const QString& username, const QString& password );

    void incompleteData( const QString& errorMessage );

private:
    Ui::LoginDialog *ui;

    QString username;

    QString password;


};

#endif // LOGINDIALOG_H
