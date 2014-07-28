#include "parametres.h"
#include "ui_parametres.h"

Parametres::Parametres(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parametres)
{
    ui->setupUi(this);

    //Init TDMS files directory
    if (readDirectoryRegister().isEmpty()) {
        directory = "";
    }
    else {
        directory = readDirectoryRegister();
    }
    ui->lineEdit->setText(directory);
    QList<QString> dbCredentials = readDbRegister();
    if(!dbCredentials.isEmpty()) {
        ui->lineEdit_2->setText(dbCredentials.at(0));
        ui->lineEdit_3->setText(dbCredentials.at(1));
        ui->lineEdit_4->setText(dbCredentials.at(2));
        ui->lineEdit_5->setText(dbCredentials.at(3));
    }
}

Parametres::~Parametres()
{
    delete ui;
}

QString Parametres::readDirectoryRegister() {
    QString destination;
    QSettings settings("METZGER","IHManchots");

    destination = settings.value("OpenTDMSDialog/destinationPath").toString();
    return destination;
}

QList<QString> Parametres::readDbRegister() {
    QVariantList reading;
    QList<QString> db;
    QSettings settings("METZGER","IHManchots");

    reading = settings.value("Database/dbCredentials").toList();
    foreach(QVariant v, reading) {
        db << v.toString();
    }

    return db;
}

void Parametres::writeRegister(QString path, QVariantList list) {
    QSettings settings("METZGER","IHManchots");
    settings.setValue("OpenTDMSDialog/destinationPath",path);
    settings.setValue("Database/dbCredentials",list);
}

void Parametres::on_pushButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this,tr("Choisissez un dossier contenant des fichiers TDMS"), directory,QFileDialog::DontUseNativeDialog);
    ui->lineEdit->setText(directory);
}

void Parametres::on_pushButton_3_clicked()
{
    QVariantList credentialsDb;
    credentialsDb << ui->lineEdit_2->text() << ui->lineEdit_3->text() << ui->lineEdit_4->text();
    if(ui->lineEdit_5->text().isEmpty()) {
        credentialsDb << "";
    }
    else {
        credentialsDb << ui->lineEdit_5->text();
    }
    writeRegister(directory,credentialsDb);
    close();
}

void Parametres::on_pushButton_2_clicked()
{
    close();
}

//void Parametres::on_pushButton_4_clicked()
//{
//    QList<QString> dbCredentials = readDbRegister();
//    QSqlDatabase db = database::dbConnect(dbCredentials);
//    if(db.open()) {
//        QMessageBox::information(this,"BDD","Vous êtes désormais connecté à la base de donnée.");
//    }
//    else {
//        QMessageBox::critical(this,"Erreur",QString("numéro de l'erreur : " + QString::number(db.lastError().number()) + "\n database erreur : " + db.lastError().databaseText() + "\ndriver erreur " + db.lastError().driverText() + "\ntype erreur : " + db.lastError().type()));
//    }
//}
