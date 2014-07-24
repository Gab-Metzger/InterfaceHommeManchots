#include "ouverturemanchot.h"
#include "ui_ouverturemanchot.h"

OuvertureManchot::OuvertureManchot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OuvertureManchot)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    authorization = false;
}

OuvertureManchot::~OuvertureManchot()
{
    delete ui;
}

void OuvertureManchot::on_submitButton_clicked()
{
    int idManchot = ui->lineEdit->text().toInt();
    QString dest = QDir::homePath() + "/.ihmanchots";

    files.Manchot_txt(QString("C:\\Users\\Gabriel\\Documents\\Courbe_Pesee\\2011-2012"),dest,idManchot);
    filenameRead = dest+"/Temp/Manchot"+QString::number(idManchot)+".txt";
    authorization = true;
    close();
}

QString OuvertureManchot::getFileName() {
    return filenameRead;
}

bool OuvertureManchot::getAuthorizationDraw() {
    return authorization;
}

void OuvertureManchot::on_cancelButton_clicked()
{
    close();
}
