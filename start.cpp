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
    connect(ui->setTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSet()));

    db = new CardDB;

    ui->browseText->setPlainText("D:\\kyogo\\sets");
    //loadSetList();
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
    ui->setTable->setSortingEnabled(false);
    for (int i=0; i < ui->setTable->rowCount(); i++) {
        QTableWidgetItem *id = new QTableWidgetItem(setList[i]->id());
        ui->setTable->setItem(i,0,id);
        id->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        QTableWidgetItem *name = new QTableWidgetItem(setList[i]->name());
        ui->setTable->setItem(i,1,name);
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }
    ui->setTable->setSortingEnabled(true);
    ui->setTable->sortByColumn(0,Qt::AscendingOrder);
}

void Start::viewSet()
{
    QMessageBox msg;
    msg.setText("Row: "+QString::number(ui->setTable->currentRow()));
    msg.exec();
}
