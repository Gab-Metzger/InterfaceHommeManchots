#include "parametres.h"
#include "ui_parametres.h"

Parametres::Parametres(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parametres)
{
    ui->setupUi(this);

    //Init TDMS files directory
    if (readRegister().isEmpty()) {
        directory = "";
    }
    else {
        directory = readRegister();
    }
    ui->lineEdit->setText(directory);
}

Parametres::~Parametres()
{
    delete ui;
}

QString Parametres::readRegister() {
    QString destination;
    QSettings settings("METZGER","IHManchots");

    destination = settings.value("OpenTDMSDialog/destinationPath").toString();
    return destination;
}

void Parametres::writeRegister(QString path) {
    QSettings settings("METZGER","IHManchots");
    settings.setValue("OpenTDMSDialog/destinationPath",path);
}

void Parametres::on_pushButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this,tr("Choisissez un dossier contenant des fichiers TDMS"), directory,QFileDialog::DontUseNativeDialog);
    ui->lineEdit->setText(directory);
}

void Parametres::on_pushButton_3_clicked()
{
    writeRegister(directory);
    close();
}

void Parametres::on_pushButton_2_clicked()
{
    close();
}
