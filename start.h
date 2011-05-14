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
    CardDB* db;

public slots:
    void getSetDir();
    void loadSetList();
};

#endif // START_H
