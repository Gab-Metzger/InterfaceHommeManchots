#ifndef PARAMETRES_H
#define PARAMETRES_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

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

private:
    Ui::Parametres *ui;
    QString directory;
    QString readRegister();
    void writeRegister(QString path);
};

#endif // PARAMETRES_H
