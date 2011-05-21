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

    ui->cardTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->cardTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->cardTable->horizontalHeader()->setStretchLastSection(true);
    connect(ui->cardTable, SIGNAL(cellChanged(int,int)), this, SLOT(editCard(int,int)));

    connect(ui->actionWrite, SIGNAL(triggered()), this, SLOT(writeChanges()));
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
    ui->setTable->setColumnCount(SETVALUES);

    disconnect(ui->setTable, SIGNAL(cellChanged(int,int)), this, SLOT(editSet(int,int)));
    ui->setTable->setSortingEnabled(false);   
    for (int i=0; i < ui->setTable->rowCount(); i++) {
        QTableWidgetItem *num = new QTableWidgetItem(QString::number(setList[i]->getSetNum()));
        ui->setTable->setItem(i,SETNUM,num);
        num->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        QTableWidgetItem *id = new QTableWidgetItem(setList[i]->getSetID());
        ui->setTable->setItem(i,SETID,id);
        id->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *name = new QTableWidgetItem(setList[i]->getSetName());
        ui->setTable->setItem(i,SETNAME,name);
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    }
    ui->setTable->setSortingEnabled(true);
    connect(ui->setTable, SIGNAL(cellChanged(int,int)), this, SLOT(editSet(int,int)));

    ui->setTable->sortByColumn(SETNAME,Qt::AscendingOrder);
    ui->setTable->setHorizontalHeaderLabels(QStringList()<<"#"<<"Set ID"<<"Set Name");
}

void Start::viewSet()
{
    ui->cardTable->clear();

    int setNum = ui->setTable->item(ui->setTable->currentRow(),SETNUM)->text().toInt();

    QList<CardInfo*> cardList = db->getCardList(setNum);
    ui->cardTable->setRowCount(cardList.size());
    ui->cardTable->setColumnCount(CARDVALUES);

    ui->cardTable->setSortingEnabled(false);
    for (int i=0; i < ui->cardTable->rowCount(); i++) {
        QTableWidgetItem *name = new QTableWidgetItem(cardList[i]->getName());
        ui->cardTable->setItem(i,NAME,name);
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *cost = new QTableWidgetItem(cardList[i]->getCost());
        ui->cardTable->setItem(i,COST,cost);
        cost->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *type = new QTableWidgetItem(cardList[i]->getType());
        ui->cardTable->setItem(i,TYPE,type);
        type->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *ah = new QTableWidgetItem(cardList[i]->getAh());
        ui->cardTable->setItem(i,AH,ah);
        ah->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);

        QTableWidgetItem *text = new QTableWidgetItem(cardList[i]->getText());
        ui->cardTable->setItem(i,TEXT,text);
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
    QString setID = ui->setTable->item(row,SETID)->text();
    int setNum = ui->setTable->item(row,SETNUM)->text().toInt();

    if (col == SETID && db->idExists(setID)) {
        QMessageBox msg;
        msg.setText("Set ID exists.");
        msg.exec();
        ui->setTable->item(row,SETID)->setText(db->getSetID(setNum));
        return;
    }

    QString setName = ui->setTable->item(row,SETNAME)->text();

    db->updateSet(setNum, setID, setName);
}

void Start::editCard(int row, int col)
{

}

void Start::writeChanges()
{
    db->writeChanges();
}
