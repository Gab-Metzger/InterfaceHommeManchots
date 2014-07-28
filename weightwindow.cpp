#include "weightwindow.h"
#include "ui_weightwindow.h"

WeightWindow::WeightWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeightWindow)
{
    ui->setupUi(this);
}

WeightWindow::~WeightWindow()
{
    delete ui;
}
