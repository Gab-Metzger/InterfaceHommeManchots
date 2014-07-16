#ifndef OUVERTUREMANCHOT_H
#define OUVERTUREMANCHOT_H

#include <QDialog>

namespace Ui {
class OuvertureManchot;
}

class OuvertureManchot : public QDialog
{
    Q_OBJECT

public:
    explicit OuvertureManchot(QWidget *parent = 0);
    ~OuvertureManchot();

private:
    Ui::OuvertureManchot *ui;
};

#endif // OUVERTUREMANCHOT_H
