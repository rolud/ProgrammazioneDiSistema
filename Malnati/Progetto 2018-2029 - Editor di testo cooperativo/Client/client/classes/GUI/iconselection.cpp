#include "include/iconselection.h"
#include "ui_iconselection.h"

IconSelection::IconSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IconSelection)
{
    ui->setupUi(this);


    QListWidgetItem *Basket= new QListWidgetItem (QIcon(":/icons/user icons/Basket.png"), "Basket");
    QListWidgetItem *Engineer= new QListWidgetItem (QIcon(":/icons/user icons/Engineer.png"), "Engineer");
    QListWidgetItem *Eve= new QListWidgetItem (QIcon(":/icons/user icons/Eve.png"), "Eve");
    QListWidgetItem *Football= new QListWidgetItem (QIcon(":/icons/user icons/Football.png"), "Football");
    QListWidgetItem *Harley= new QListWidgetItem (QIcon(":/icons/user icons/Harley.png"), "Harley");
    QListWidgetItem *Love= new QListWidgetItem (QIcon(":/icons/user icons/Love.png"), "Love");
    QListWidgetItem *Motorsport= new QListWidgetItem (QIcon(":/icons/user icons/Motorsport.png"), "Motorsport");
    QListWidgetItem *Music= new QListWidgetItem (QIcon(":/icons/user icons/Music.png"), "Music");
    QListWidgetItem *Nature= new QListWidgetItem (QIcon(":/icons/user icons/Nature.png"), "Nature");
    QListWidgetItem *Navy= new QListWidgetItem (QIcon(":/icons/user icons/Navy.png"), "Navy");
    QListWidgetItem *Prize= new QListWidgetItem (QIcon(":/icons/user icons/Prize.png"), "Prize");
    QListWidgetItem *Research= new QListWidgetItem (QIcon(":/icons/user icons/Research.png"), "Research");
    QListWidgetItem *Robot= new QListWidgetItem (QIcon(":/icons/user icons/Robot.png"), "Robot");
    QListWidgetItem *Soccer= new QListWidgetItem (QIcon(":/icons/user icons/Soccer.png"), "Soccer");
    QListWidgetItem *Star= new QListWidgetItem (QIcon(":/icons/user icons/Star.png"), "Star");
    QListWidgetItem *Summer= new QListWidgetItem (QIcon(":/icons/user icons/Summer.png"), "Summer");
    QListWidgetItem *Tennis= new QListWidgetItem (QIcon(":/icons/user icons/Tennis.png"), "Tennis");
    QListWidgetItem *Trip= new QListWidgetItem (QIcon(":/icons/user icons/Trip.png"), "Trip");
    QListWidgetItem *Videogame= new QListWidgetItem (QIcon(":/icons/user icons/Videogame.png"), "Videogame");
    QListWidgetItem *Yoga= new QListWidgetItem (QIcon(":/icons/user icons/Yoga.png"), "Yoga");

    ui->IconList->addItem(Basket);
    ui->IconList->addItem(Engineer);
    ui->IconList->addItem(Eve);
    ui->IconList->addItem(Football);
    ui->IconList->addItem(Harley);
    ui->IconList->addItem(Love);
    ui->IconList->addItem(Motorsport);
    ui->IconList->addItem(Music);
    ui->IconList->addItem(Nature);
    ui->IconList->addItem(Navy);
    ui->IconList->addItem(Prize);
    ui->IconList->addItem(Research);
    ui->IconList->addItem(Robot);
    ui->IconList->addItem(Soccer);
    ui->IconList->addItem(Star);
    ui->IconList->addItem(Summer);
    ui->IconList->addItem(Tennis);
    ui->IconList->addItem(Trip);
    ui->IconList->addItem(Videogame);
    ui->IconList->addItem(Yoga);


    ui->IconList->setEditTriggers(QAbstractItemView::NoEditTriggers);


}

IconSelection::~IconSelection()
{
    delete ui;
}

void IconSelection::on_IconList_itemDoubleClicked(QListWidgetItem *item)
{
   icon= ui->IconList->row(item)+1;

   //iconSelectedPath= ":/icons/user icons/" + item->text() + ".png";

   emit iconSelected(icon);
   this->hide();
}
