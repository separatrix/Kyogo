#include "start.h"
#include "ui_start.h"
#include <QtGui>
#include "carddb.h"

Start::Start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(getSetDir()));
    connect(ui->browseText, SIGNAL(textChanged()), this, SLOT(loadSetList()));

    ui->setTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->setTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->setTable->horizontalHeader()->setStretchLastSection(true);
    connect(ui->setTable, SIGNAL(itemSelectionChanged()), this, SLOT(viewSet()));
    connect(ui->setTable, SIGNAL(cellChanged(int,int)), this, SLOT(editSet(int,int)));

    ui->cardTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->cardTable->horizontalHeader()->setStretchLastSection(true);

    connect(ui->actionNewSet, SIGNAL(triggered()), this, SLOT(newSet()));

    db = new CardDB;

    ui->browseText->setPlainText("D:\\kyogo\\sets");
}

Start::~Start()
{
    delete ui;
}

void Start::getSetDir()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, "Open Set Directory");
    ui->browseText->setPlainText(path);
}

void Start::loadSetList()
{
    db->empty();

    db->loadSetsFromDir(ui->browseText->toPlainText());
    ui->setTable->clear();

    QList<SetInfo*> setList = db->getSetList();
    ui->setTable->setRowCount(setList.size());
    ui->setTable->setColumnCount(2);

    disconnect(ui->setTable, SIGNAL(cellChanged(int,int)), this, SLOT(editSet(int,int)));
    ui->setTable->setSortingEnabled(false);   
    for (int i=0; i < ui->setTable->rowCount(); i++) {
        QTableWidgetItem *id = new QTableWidgetItem(setList[i]->getSetID());
        ui->setTable->setItem(i,0,id);
        id->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        QTableWidgetItem *name = new QTableWidgetItem(setList[i]->getSetName());
        ui->setTable->setItem(i,1,name);
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    }
    ui->setTable->setSortingEnabled(true);
    connect(ui->setTable, SIGNAL(cellChanged(int,int)), this, SLOT(editSet(int,int)));

    ui->setTable->sortByColumn(0,Qt::AscendingOrder);    
    ui->setTable->setHorizontalHeaderLabels(QStringList()<<"Set ID"<<"Set Name");
}

void Start::viewSet()
{
    ui->cardTable->clear();

    QString setID = ui->setTable->item(ui->setTable->currentRow(),0)->text();

    QList<CardInfo*> cardList = db->getCardList(setID);
    ui->cardTable->setRowCount(cardList.size());
    ui->cardTable->setColumnCount(5);

    ui->cardTable->setSortingEnabled(false);
    for (int i=0; i < ui->cardTable->rowCount(); i++) {
        QTableWidgetItem *name = new QTableWidgetItem(cardList[i]->getName());
        ui->cardTable->setItem(i,0,name);
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *cost = new QTableWidgetItem(cardList[i]->getCost());
        ui->cardTable->setItem(i,1,cost);
        cost->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *type = new QTableWidgetItem(cardList[i]->getType());
        ui->cardTable->setItem(i,2,type);
        type->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *ah = new QTableWidgetItem(cardList[i]->getAh());
        ui->cardTable->setItem(i,3,ah);
        ah->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *text = new QTableWidgetItem(cardList[i]->getText());
        ui->cardTable->setItem(i,4,text);
        text->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    }
    ui->cardTable->setSortingEnabled(true);

    ui->cardTable->setHorizontalHeaderLabels(QStringList()<<"Name"<<"Cost"<<"Type"<<"A/H"<<"Text");
}

void Start::newSet()
{

}

void Start::editSet(int row, int col)
{
    if (col == 1) {
        QString setID = ui->setTable->item(row,0)->text();
        QString setName = ui->setTable->item(row,1)->text();
        db->updateSetName(setID, setName);
    }
}
