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

    db = new CardDB;

    ui->setTable->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    for (int i=0; i<ui->setTable->rowCount(); i++) {
        ui->setTable->setItem(i,0,new QTableWidgetItem(setList[i]->id()));
        ui->setTable->setItem(i,1,new QTableWidgetItem(setList[i]->name()));
    }
    ui->setTable->setSortingEnabled(true);
    ui->setTable->sortByColumn(0,Qt::AscendingOrder);
}
