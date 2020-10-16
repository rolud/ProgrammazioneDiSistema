#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

#include<QFile>
#include<QFileDialog>
#include<QTextStream>
#include<QMessageBox>
#include<QPrinter>
#include<QPrintDialog>
#include<QFont>
#include <QListWidget>
#include <QStandardItem>

#include "include/algorithmcrdt.h"
#include "include/message.h"
#include "include/charactermessage.h"
#include "include/usermessage.h"
#include "include/errordialog.h"

namespace Ui {
class editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor();

    AlgorithmCRDT &getAlgorithm();

    void setClientId( const int clientId );
    void setCurrentDocumentName( const QString& documentName );

    QString getCurrentDocumentUrl();

signals:
    // Quando l'utente fa un'azione, questa viene convertita in Message ed inviata
    // al Client che lo converte in Json e lo manda al server con il socket.
    void newCharacterMessage( const CharacterMessage& message );
    void displayURL (const QString& URL);
    void requestLogout (const QString& username);

public slots:
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    //void on_actionSave_triggered();
    // void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionBold_triggered();
    void on_actionUnderlined_triggered();
    void on_actionItalic_triggered();
    void on_contents_change(int position, int deleted, int added);


    void onFontFamilySelected (const QFont& font);
    void onFontSizeSelected (const int& size);

    void onCharacterMessageReceived( const CharacterMessage& characterMessage );
    void onUserMessageReceived( const UserMessage& userMessage );
    void onConfigurationEnded();

    void onDocumentSelectedSuccessfully( const QString& documentName );

    void setUsername (const QString& username );
    void setUserData( const QString& username, const int icon );

    void setIcon (int icon);
    void setBackground (int background);

    QString getUsername();

    void clear();

    QString getColorName( const int color );

private slots:


    void on_actionAlign_Left_triggered();

    void on_actionAlign_Right_triggered();

    void on_actionJustified_triggered();

    void on_actionCentered_triggered();

    void on_actionCreate_PDF_File_triggered();

    void on_actionShare_triggered();

    void on_actionLogout_triggered();




    void on_actionUserColors_triggered();

private:
    Ui::editor *ui;
    QString currentFile="";
    AlgorithmCRDT crdt;
    std::string old_text = "";
    std::vector<QTextCharFormat> old_text_format{};
    bool changing_alignment_locale = false;
    bool changing_alignment_remote = false;
    std::map<int, QString> active_clients;
    int assignColor(int root);
    bool highlighting_authors = false;
    bool changing_highlight = false;
    QString user;

    QString currentDocumentName;

    int icon;

    QString getIconPath( const int icon );

    void saveTextFormat();

};








#endif // EDITOR_H
