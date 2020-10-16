#ifndef DOCUMENTHANDLER_H
#define DOCUMENTHANDLER_H

#include <QMainWindow>
#include <QListWidget>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QDebug>


namespace Ui {
class DocumentHandler;
}

class DocumentHandler : public QMainWindow
{
    Q_OBJECT


public:
    explicit DocumentHandler(QWidget *parent = nullptr);
    ~DocumentHandler();

public slots:

    void on_SelectByURL_returnPressed();

    void onDocumentsListReceived( const QList< QString > documentsList );

    void on_CreateNewDocument_returnPressed();

    void on_DocumentList_doubleClicked(const QModelIndex &index);

    void setUsername (const QString& username);

    void clear();


signals:
    void createNewDocument( const QString& documentTitle );
    void selectByURL( const QString& documentURL );
    void openDocument(const QString& document);
    void updateProfile (const QString& title);


private slots:
    void on_EditProfilePushButton_clicked();

    void on_UpdateProfilePushButton_clicked();

private:
    Ui::DocumentHandler *ui;
    QString user;
    QString title;
    QString URL;

    QStandardItemModel *documentListModel = new QStandardItemModel(this);
};

#endif // DOCUMENTHANDLER_H
