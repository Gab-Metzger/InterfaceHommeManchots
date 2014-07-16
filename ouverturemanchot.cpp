#include "ouverturemanchot.h"
#include "ui_ouverturemanchot.h"

OuvertureManchot::OuvertureManchot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OuvertureManchot)
{
    ui->setupUi(this);
}

OuvertureManchot::~OuvertureManchot()
{
    delete ui;
}
