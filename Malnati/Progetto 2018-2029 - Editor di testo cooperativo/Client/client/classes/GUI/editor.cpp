#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QObject>
#include <QtNetwork/QHostAddress>
#include <string>
#include <QByteArray>
// #include <conio.h>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QTextCharFormat>
#include <Qt>
#include <QWidgetAction>
#include <QFontComboBox>
#include <QSpinBox>
#include "include/editor.h"
#include "ui_editor.h"
#include "include/client.h"
#include "include/message.h"
#include "include/charactermessage.h"
#include "include/usermessage.h"



Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::editor)
{
    ui->setupUi(this);


    QFontComboBox *fontBox = new QFontComboBox(ui->toolBar);
    QWidgetAction *fontAction = new QWidgetAction(ui->toolBar);
    fontAction->setDefaultWidget(fontBox);
    fontBox->setCurrentFont(QFont("Arial"));
    ui->toolBar->addAction(fontAction);


    QSpinBox *spinBox = new QSpinBox(ui->toolBar);
    QWidgetAction *spinAction = new QWidgetAction(ui->toolBar);
    spinAction->setDefaultWidget(spinBox);
    spinBox->setValue(10);
    ui->toolBar->addAction(spinAction);

    QAction *logout= new QAction (QIcon(":/icons/icons/logout.png"), "Logout");
    ui->toolBar->addAction (logout);
    QObject::connect( logout, SIGNAL(triggered()), this, SLOT(on_actionLogout_triggered()));




    QObject::connect( this->ui->textEdit->document(), &QTextDocument::contentsChange, this, &Editor::on_contents_change );

    QObject::connect(fontBox, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(onFontFamilySelected (const QFont&)));
    QObject::connect(spinBox, SIGNAL(valueChanged(const int&)), this, SLOT (onFontSizeSelected(const int&)));



    /*QPixmap pixmap(":/icons/icons/logout.png");
    QIcon ButtonIcon(pixmap);

    ui->LogoutPushButton->setIcon(ButtonIcon);
    ui->LogoutPushButton->setIconSize(pixmap.rect().size());*/


}




Editor::~Editor()
{
    delete ui;
}

AlgorithmCRDT &Editor::getAlgorithm() {
    return this->crdt;
}


void Editor::setUsername (const QString& username ){

    this->user=username;
}

QString Editor::getUsername(){

    return user;
}

void Editor::setIcon (int icon){

    this->icon=icon;



}

void Editor::setBackground (int background)
{
    (void)background;
}


void Editor::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void Editor::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void Editor::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void Editor::on_actionUndo_triggered()
{
    //ui->textEdit->undo();
}

void Editor::on_actionRedo_triggered()
{
    //ui->textEdit->redo();
}

void Editor::onFontFamilySelected (const QFont& font){

    QTextCharFormat format{};

    format.setFont(font);

    ui->textEdit->mergeCurrentCharFormat(format);

}


void Editor::onFontSizeSelected (const int& size){

    QTextCharFormat format{};

   format.setFontPointSize(size);

    ui->textEdit->mergeCurrentCharFormat(format);

}




/*void Editor::on_actionSave_triggered()
{
    QString fileName= QFileDialog::getSaveFileName(this, "Save as");


    QFile file (fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning (this, "Warninng", "Cannot save file:"+ file.errorString());
    }

    currentFile=fileName;

    setWindowTitle(fileName);

    QTextStream out(&file);

    QString text=ui->textEdit->toPlainText();

    out << text;

    file.close();

}*/

/*void Editor::on_actionOpen_triggered()
{
    QString fileName= QFileDialog::getOpenFileName(this, "Open the file");


    QFile file (fileName);

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning (this, "Warninng", "Cannot save file:"+ file.errorString());
    }

    currentFile=fileName;

    setWindowTitle(fileName);

    QTextStream in(&file);

    QString text=in.readAll();

   ui->textEdit->setText(text);

    file.close();
}*/

void Editor::on_actionExit_triggered()
{
    QApplication::quit();
}

void Editor::on_actionBold_triggered()
{

    QTextCharFormat format{};

    if ( ui->actionBold->isChecked() )
        format.setFontWeight(QFont::Bold);
    else
        format.setFontWeight(QFont::Normal);

    ui->textEdit->mergeCurrentCharFormat(format);
}

void Editor::on_actionUnderlined_triggered()
{
    QTextCharFormat format{};

    if ( ui->actionUnderlined->isChecked() )
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    else
        format.setUnderlineStyle(QTextCharFormat::NoUnderline);

    ui->textEdit->mergeCurrentCharFormat(format);
}

void Editor::on_actionItalic_triggered()
{
    QTextCharFormat format{};

    if ( ui->actionItalic->isChecked() )
        format.setFontItalic(QTextCharFormat::FontItalic);
    else
        format.setFontItalic(QTextCharFormat::NoUnderline);

    ui->textEdit->mergeCurrentCharFormat(format);
}


void Editor::on_contents_change(int position, int deleted, int added) {
    std::string text = ui->textEdit->toPlainText().toStdString();
    QString q_text = ui->textEdit->toPlainText();

    /// bug QTextDocument::contentsChange
    if (static_cast<unsigned int>(deleted) >= this->old_text.size() && added > 1) {
        deleted--;
        added--;
    }

    // todo fix selection edits
    //qDebug() << "LAST OP SOURCE " << this->crdt.getLastOperationSource();
    //qDebug() << "changing alignment remote : " << this->changing_alignment_remote;
    if (!this->changing_alignment_remote && !this->changing_highlight) {
        if (this->crdt.getLastOperationSource() != AlgorithmCRDT::OperationSource::REMOTE) {

            //std::cout << "IN ON CHANGE" << std::endl;
            //qDebug() << "position " << position << " | deleted " << deleted << " | added " << added;
            //qDebug() << "actual text" << ui->textEdit->toPlainText() << " || size " << text.size();


            //qDebug() << "changing alignment locale : " << this->changing_alignment_locale;
            if (this->changing_alignment_locale) {
                deleted--;
                added--;
            }

            if (deleted != 0) {
                // DELETION
                //std::cout << "DELETION AT " << position << std::endl;
                for (int i = 0; i<deleted; i++) {
                    try {
                        const CharacterMessage message = this->crdt.localErase(position);
                        emit this->newCharacterMessage( message );
                    } catch (std::invalid_argument e) {
                        qDebug() << e.what();
                    }
                }

            }

            if (added != 0) {
                // INSERTION

                //std::cout << "INSERTION AT " << position << std::endl;
                auto format = ui->textEdit->currentCharFormat();
                auto alignment = ui->textEdit->alignment();
                auto fontFamily = ui->textEdit->fontFamily();



                for (int i = 1; i<=added;i++) {
                    QTextCharFormat f;
                    if (this->changing_alignment_locale) {
                        //qDebug() << "|||||||||||||||||CHANGE ALIGNEMENT FORMAT|||||||||||||||||";
                        f = old_text_format[position+i-1];
                    } else f = format;
                    //qDebug() << " ==> char " << text[position+i-1] << " at " << position;

                    const CharacterMessage message = this->crdt.localInsert(q_text.at(position+i-1), static_cast<unsigned int>(position + i - 1), f, alignment);
                    emit this->newCharacterMessage( message );
                }



                // fix background color if we are editing
                // inside a block of another user
                changing_highlight = true;
                auto cur = ui->textEdit->textCursor();
                auto prevCurPos = cur.position();
                int diff = abs(deleted - added);
                cur.setPosition(position, QTextCursor::MoveAnchor);
                cur.setPosition(position + diff, QTextCursor::KeepAnchor);
                QColor color = getColorName(0);
                auto ccf = ui->textEdit->currentCharFormat();
                ccf.setBackground(color);
                cur.mergeCharFormat(ccf);
                cur.setPosition(prevCurPos, QTextCursor::MoveAnchor);
                changing_highlight = false;

            }


            //std::cout << "ACTUALLY IN ALGORITHM STRUCTURE : " << this->crdt.to_string() << std::endl;
            //std::cout << "AFTER INSERT cursor position now " << ui->textEdit->textCursor().position() << std::endl;
            if (this->changing_alignment_locale) {
                this->changing_alignment_locale = false;
            }
            old_text = ui->textEdit->toPlainText().toStdString();
            saveTextFormat();
        } else {
            this->crdt.setLastOperationSource(AlgorithmCRDT::OperationSource::NONE);
        }
    } else {
        //qDebug() << "|||||||||||||||||CHANGE ALIGNEMENT FORMAT REMOTE|||||||||||||||||";
        this->changing_alignment_remote = false;
    }
}

// Intercetta il segnale messageReceived del Client, scatenato quando riceve un nuovo messaggio sul socket
void Editor::onCharacterMessageReceived( const CharacterMessage& message )
{
    QTextCursor prev = ui->textEdit->textCursor();
    //qDebug() << "cursor position " << prev.position();
    if (message.getOperationType() == CharacterMessage::OperationType::INSERT) {
        int index=  static_cast<int>(crdt.remoteInsert(message.getSymbol()));
        //qDebug() << "after crdt insertion - index  position " << index;
        if (index > prev.position())
            for (int i = 0; i < std::abs(index - prev.position()); i++){
                ui->textEdit->moveCursor(QTextCursor::Right);
            }
        else if (index < prev.position())
            for (int i = 0; i < std::abs(index - prev.position()); i++) {
                ui->textEdit->moveCursor(QTextCursor::Left);
            }

        QTextCharFormat newFormat{};

        // set italic
        if ( message.getSymbol().isItalic() ) {
           newFormat.setFontItalic(true);
           newFormat.setFontItalic(QTextCharFormat::FontItalic);
        } else {
            newFormat.setFontItalic(false);
            newFormat.setFontItalic(QTextCharFormat::NoUnderline);
        }

        // set underline
        if ( message.getSymbol().isUnderline() ) {
            newFormat.setFontUnderline(true);
            newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        } else {
            newFormat.setFontUnderline(false);
            newFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
        }

        // set bold
        if ( message.getSymbol().getWeight() == 75 ) { // todo weight change with font? check
            newFormat.setFontWeight(QFont::Bold);
        } else {
            newFormat.setFontWeight(QFont::Normal);
        }
        newFormat.setFontWeight(message.getSymbol().getWeight());

        newFormat.setFontFamily(message.getSymbol().getFontFamily());
        newFormat.setFontPointSize(message.getSymbol().getFontSize());

        if (this->highlighting_authors) {
            auto r = this->active_clients.find(message.getClientId());
            if (r != this->active_clients.end()) {
                QColor color = r->second;
                newFormat.setBackground(color);
            } else {
                QColor color = getColorName(0);
                newFormat.setBackground(color);
            }
        }

        auto oldFormat = ui->textEdit->currentCharFormat();
        ui->textEdit->setCurrentCharFormat(newFormat);


        auto newAlignment = message.getSymbol().getAlignment();
        if (ui->textEdit->alignment() != newAlignment) {
            this->changing_alignment_remote = true;
            QTextBlockFormat blockFormat{};
            blockFormat.setAlignment(newAlignment);
            ui->textEdit->textCursor().setBlockFormat(blockFormat);
        }

        ui->textEdit->insertPlainText(QString(message.getSymbol().getValue()));

        ui->textEdit->setCurrentCharFormat(oldFormat);
        ui->textEdit->textCursor().mergeCharFormat(oldFormat);
    } else if (message.getOperationType() == CharacterMessage::OperationType::DELETE) {
        // TODO implement deletion
        int index = static_cast<int>(this->crdt.remoteErase(message.getSymbol()));
        //qDebug() << "after xrdt deletion - index  position " << index;
        if (index > prev.position())
            for (int i = 0; i < std::abs(index - prev.position()); i++){
                ui->textEdit->moveCursor(QTextCursor::Right);
            }
        else if (index < prev.position())
            for (int i = 0; i < std::abs(index - prev.position()); i++) {
                ui->textEdit->moveCursor(QTextCursor::Left);
            }
        ui->textEdit->textCursor().deleteChar();
        //qDebug() << "cursor position (after) " << ui->textEdit->textCursor().position();
    }
    ui->textEdit->setTextCursor(prev);
    //qDebug() << "actual text" << ui->textEdit->toPlainText() << " || size " << ui->textEdit->toPlainText().size();
    //std::cout << "onCharMsgReceived -> actual in alg struct -> " << this->crdt.to_string() << std::endl;
}

void Editor::onUserMessageReceived( const UserMessage& message )
{
    if ( message.getEventType() == UserMessage::EventType::CONNECTED )
    {
        qDebug() << "User Message received:" << message.getUserName() << "connected with icon" << message.getIcon();

        QString iconPath = this->getIconPath( message.getIcon() );
        QListWidgetItem *item= new QListWidgetItem (QIcon(iconPath), message.getUserName());

        int colorId= assignColor(message.getClientId());
        QString colorName = getColorName(colorId);
        QColor color = colorName;
            item->setBackground(color);

        ui->ConnectedUsers->addItem(item);

        active_clients.insert(std::pair<int,QString>(message.getClientId(), colorName));

        //userListModel->appendRow(new QStandardItem(QIcon(":/icons/user icons/smile.png"), message.getUserName()));

        //ui->ConnectedUsers->setModel(userListModel);


    }

    else
    {
        qDebug() << "User Message received:" << message.getUserName() << "disconnected.";


        /*auto items= userListModel->findItems(message.getUserName());

        userListModel->removeRow(items[0]->row());


        ui->ConnectedUsers->setModel(userListModel);*/


         int itemCounts = ui->ConnectedUsers->count();
         for (int i = 0; i < itemCounts; i++) {
             auto item = ui->ConnectedUsers->item(i);
             if (item->text() == message.getUserName()) {
                 ui->ConnectedUsers->takeItem(i);
                 break;
             }
        }

         for(std::map<int,QString>::iterator it=active_clients.begin(); it!=active_clients.end(); it++) {
             if(it->first == message.getClientId()) {
                 active_clients.erase(it);
                 break;
             }
         }
    }

}

void Editor::onConfigurationEnded() {
    this->ui->labelLoading->setVisible(false);
    this->ui->textEdit->setVisible(true);
    this->crdt.setLastOperationSource(AlgorithmCRDT::NONE);
}


void Editor::setClientId( const int clientId )
{
    this->crdt.setSiteID( clientId );
}


void Editor::on_actionAlign_Left_triggered()
{
    this->changing_alignment_locale = true;
    ui->textEdit->setAlignment(Qt::AlignLeft);
}

void Editor::on_actionAlign_Right_triggered()
{
    this->changing_alignment_locale = true;
    ui->textEdit->setAlignment(Qt::AlignRight);
}

void Editor::on_actionJustified_triggered()
{
    this->changing_alignment_locale = true;
    ui->textEdit->setAlignment(Qt::AlignJustify);
}

void Editor::on_actionCentered_triggered()
{
    this->changing_alignment_locale = true;
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void Editor::on_actionCreate_PDF_File_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);

    QString filename = QFileDialog::getSaveFileName(nullptr, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(filename).suffix().isEmpty()) { filename.append(".pdf"); }
    printer.setOutputFileName(filename);

    ui->textEdit->document()->print(&printer);
}



QString Editor::getCurrentDocumentUrl()
{
    if ( this->currentDocumentName.isNull() || this->currentDocumentName.isEmpty() )
    {
        return "";
    }

    return "url://" + this->currentDocumentName;
}

void Editor::onDocumentSelectedSuccessfully( const QString& documentName )
{
    this->currentDocumentName = documentName;

    QString iconPath = this->getIconPath( this->icon );
    QListWidgetItem *item= new QListWidgetItem (QIcon( iconPath ), "You (" + this->getUsername() + ")" );
    ui->ConnectedUsers->addItem(item);

    this->show();
    this->ui->textEdit->setVisible(false);
    this->ui->labelLoading->setVisible(true);

}

void Editor::on_actionShare_triggered()
{
    QString message{ "Share this URL with your friends to give them access to this document:" "\n\n" + getCurrentDocumentUrl() };
    emit this->displayURL(message);
}

void Editor::on_actionLogout_triggered()
{
    emit this->requestLogout(user);
}

/*void Editor::on_LogoutPushButton_clicked()
{
    this->on_actionLogout_triggered();
}*/

void Editor::setUserData(const QString& username, const int icon)
{
    this->setUsername(username);
    this->setIcon(icon);
}


QString Editor::getIconPath( const int icon )
{
    QString iconPath{};

    switch ( icon )
    {
        case 1:
            iconPath= ":/icons/user icons/Basket.png";
            break;
        case 2:
            iconPath= ":/icons/user icons/Engineer.png";
            break;
        case 3:
            iconPath= ":/icons/user icons/Eve.png";
            break;
        case 4:
            iconPath= ":/icons/user icons/Football.png";
            break;
        case 5:
            iconPath= ":/icons/user icons/Harley.png";
            break;
        case 6:
            iconPath= ":/icons/user icons/Love.png";
            break;
        case 7:
            iconPath= ":/icons/user icons/Motorsport.png";
            break;
        case 8:
            iconPath= ":/icons/user icons/Music.png";
            break;
        case 9:
            iconPath= ":/icons/user icons/Nature.png";
            break;
        case 10:
            iconPath= ":/icons/user icons/Navy.png";
            break;
        case 11:
            iconPath= ":/icons/user icons/Prize.png";
            break;
        case 12:
            iconPath= ":/icons/user icons/Research.png";
            break;
        case 13:
            iconPath= ":/icons/user icons/Robot.png";
            break;
        case 14:
            iconPath= ":/icons/user icons/Soccer.png";
            break;
        case 15:
            iconPath= ":/icons/user icons/Star.png";
            break;
        case 16:
            iconPath= ":/icons/user icons/Summer.png";
            break;
        case 17:
            iconPath= ":/icons/user icons/Tennis.png";
            break;
        case 18:
            iconPath= ":/icons/user icons/Trip.png";
            break;
        case 19:
            iconPath= ":/icons/user icons/Videogame.png";
            break;
        default:
            iconPath= ":/icons/user icons/Yoga.png";
            break;
    }

    return iconPath;
}




void Editor::clear(){

    currentFile="";
    old_text = "";
    changing_alignment_locale = false;
    changing_alignment_remote = false;
    user="";
    currentDocumentName="";
    icon=0;
    ui->ConnectedUsers->clear();


    QObject::disconnect( this->ui->textEdit->document(), &QTextDocument::contentsChange, this, &Editor::on_contents_change );

    ui->textEdit->clear();
    this->crdt.clear();
    this->active_clients.clear();

    QObject::connect( this->ui->textEdit->document(), &QTextDocument::contentsChange, this, &Editor::on_contents_change );

}

void Editor::saveTextFormat() {
    this->old_text_format.clear();
    auto cursor_position = this->ui->textEdit->textCursor().position();
    //std::cout <<"BOLD STATUS -> ";
    QTextCursor cursor(this->ui->textEdit->document());
    cursor.movePosition(QTextCursor::Start);
    for (int i = 0; i < this->ui->textEdit->toPlainText().length(); i++) {
        cursor.movePosition(QTextCursor::Right);
        this->old_text_format.push_back(cursor.charFormat());
        // std::cout << old_text_format[i].fontWeight() << "("<<cursor.position()<<"); ";
    }
    // std::cout << std::endl;
    this->ui->textEdit->textCursor().setPosition(cursor_position);

}



void Editor::on_actionUserColors_triggered()
{
    changing_highlight = true;

    highlighting_authors = !highlighting_authors;

    saveTextFormat();

    if (highlighting_authors) {

        std::vector<int> textAuthor = this->crdt.getTextAuthors();

        // qDebug() << "text author size: " << textAuthor.size();
        // qDebug() << "       text size: " << this->ui->textEdit->toPlainText().size();
        QTextCursor cursor(this->ui->textEdit->document());
        for (unsigned int i = 0;  i < textAuthor.size(); i++) {
            QString colorName;
            auto r = this->active_clients.find(textAuthor[i]);
            if (r != this->active_clients.end())
                colorName = r->second;
            else
                colorName = getColorName(0);

            QColor color = colorName;
            cursor.setPosition(static_cast<int>(i), QTextCursor::MoveAnchor);
            cursor.setPosition(static_cast<int>(i) + 1, QTextCursor::KeepAnchor);

            QTextCharFormat tcf = old_text_format[i];
            tcf.setBackground(color);
            cursor.mergeCharFormat(tcf);
        }
    } else {

        QTextCursor cursor(this->ui->textEdit->document());
        QColor color = getColorName(0);
        for (int i = 0;  i < this->ui->textEdit->toPlainText().size(); i++) {
            cursor.setPosition(i, QTextCursor::MoveAnchor);
            cursor.setPosition(i + 1, QTextCursor::KeepAnchor);

            QTextCharFormat tcf = old_text_format[static_cast<unsigned int>(i)];
            tcf.setBackground(color);
            cursor.mergeCharFormat(tcf);
        }

    }

    changing_highlight = false;
}

QString Editor::getColorName( const int color )
{
    QString colorName{};

    switch ( color )
    {
        case 0:
            colorName= "White";
            break;
        case 1:
            colorName= "DarkOrange";
            break;
        case 2:
            colorName= "Cyan";
            break;
        case 3:
            colorName= "LawnGreen";
            break;
        case 4:
            colorName= "Gold";
            break;
        case 5:
            colorName= "Lime";
            break;
        case 6:
            colorName= "MediumSpringGreen";
            break;
        case 7:
            colorName= "RoyalBlue";
            break;
        case 8:
            colorName= "DodgerBlue";
            break;
        case 9:
            colorName= "LightCoral";
            break;
        case 10:
            colorName= "BlueViolet";
            break;
        case 11:
            colorName= "Violet";
            break;
        case 12:
            colorName= "Magenta";
            break;
        case 13:
            colorName= "Red";
            break;
        case 14:
            colorName= "Chartreuse";
            break;
        case 15:
            colorName= "Yellow";
            break;
        case 16:
            colorName= "SandyBrown";
            break;
        case 17:
            colorName= "LightGreen";
            break;
        case 18:
            colorName= "LightSeaGreen";
            break;
        case 19:
            colorName= "Turquoise";
            break;
        case 20:
            colorName= "DeepSkyBlue";
            break;
        case 21:
            colorName= "Blue";
            break;
        case 22:
            colorName= "Indigo";
            break;
        case 23:
            colorName= "Orchid";
            break;
        default:
            colorName= "WhiteSmoke";
            break;
    }

    return colorName;
}

int Editor::assignColor(int root) {
    (void)root;

    int col = (active_clients.size() + 1) % 24;

    /*
    int col = root%24;
    int increments=0;
    while(active_clients.find(col)!=active_clients.end()) {
        col++;
        increments++;
        if(increments==23) { col=0; break; }
    }
    */
    return col;
}
