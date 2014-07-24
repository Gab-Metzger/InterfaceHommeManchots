#ifndef OUVERTUREMANCHOT_H
#define OUVERTUREMANCHOT_H

#include <QDialog>
#include <QDebug>
#include "opentdms.h"
#include "database.h"

namespace Ui {
class OuvertureManchot;
}

class OuvertureManchot : public QDialog
{
    Q_OBJECT

public:
    explicit OuvertureManchot(QWidget *parent = 0);
    ~OuvertureManchot();
    QString getFileName();
    bool getAuthorizationDraw();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::OuvertureManchot *ui;
    OpenTdms files;
    QString filenameRead;
    QString directory;
    bool authorization;
    QString readRegister();
};

#endif // OUVERTUREMANCHOT_H
