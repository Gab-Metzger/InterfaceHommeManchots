#ifndef PARAMETRES_H
#define PARAMETRES_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QVariantList>
#include <QList>
#include <QSqlDatabase>
#include <QMessageBox>
#include "database.h"

namespace Ui {
class Parametres;
}

class Parametres : public QDialog
{
    Q_OBJECT

public:
    explicit Parametres(QWidget *parent = 0);
    ~Parametres();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    //void on_pushButton_4_clicked();

private:
    Ui::Parametres *ui;
    QString directory;
    QString readDirectoryRegister();
    QList<QString> readDbRegister();
    void writeRegister(QString path, QVariantList list);
};

#endif // PARAMETRES_H
