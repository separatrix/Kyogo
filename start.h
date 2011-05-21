#ifndef START_H
#define START_H

#include <QMainWindow>
#include "carddb.h"

namespace Ui {
    class Start;
}

class Start : public QMainWindow
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();

private:
    Ui::Start *ui;
    CardDB *db;
    enum { SETNUM, SETID, SETNAME, SETVALUES };
    enum { NAME, COST, TYPE, AH, TEXT, CARDVALUES };

public slots:
    void getSetDir();
    void loadSetList();
    void viewSet();
    void newSet();
    void editSet(int row, int col);
    void editCard(int row, int col);
    void writeChanges();
};

#endif // START_H
