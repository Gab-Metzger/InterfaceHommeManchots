#include "ouverturemanchot.h"
#include "ui_ouverturemanchot.h"

OuvertureManchot::OuvertureManchot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OuvertureManchot)
{
    ui->setupUi(this);
    ui->idLineEdit->setFocus();
    directory = ReadRegister::readInRegister("OpenTDMSDialog/destinationPath").toString();
    authorization = false;
}

OuvertureManchot::~OuvertureManchot()
{
    delete ui;
}

void OuvertureManchot::on_submitButton_clicked()
{
    int idManchot = ui->idLineEdit->text().toInt();
    int nbResult;
    QString dest = QDir::homePath() + "/.ihmanchots";
    nbResult = files.ManchotTxt(directory,dest,idManchot);

    filenameRead = dest+"/Temp/Manchot"+QString::number(idManchot)+".txt";

    if(nbResult == 1) {
       authorization = true;
       close();
    }
    else if (nbResult == -2) {
        QMessageBox::critical(this,"BDD","Erreur de connexion à la base de donnée.\nVeuillez entrer vos identifiants dans le menu Outils->Options...");
    }
    else {
        QMessageBox::critical(this,"Erreur","Il n'y a pas de courbes pour ce manchot.");
    }
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
