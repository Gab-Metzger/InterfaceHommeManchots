#ifndef UPDATEDATABASE_H
#define UPDATEDATABASE_H

#include <QDialog>
#include <QDebug>
#include <QFutureWatcher>
#include "opentdms.h"
#include "resultat.h"
#include "readregister.h"

namespace Ui {
class UpdateDatabase;
}

class UpdateDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDatabase(QWidget *parent = 0);
    ~UpdateDatabase();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();
    void slot_finished();

private:
    Ui::UpdateDatabase *ui;
    OpenTdms ot;
    QFutureWatcher<void> FutureWatcher;
};

#endif // UPDATEDATABASE_H
