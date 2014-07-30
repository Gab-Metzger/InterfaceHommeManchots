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
    ui->tdmsFileLineEdit->setText(directory);
    QList<QString> dbCredentials = readDbRegister();
    if(!dbCredentials.isEmpty()) {
        ui->domainNameLineEdit->setText(dbCredentials.at(0));
        ui->databaseNameLineEdit->setText(dbCredentials.at(1));
        ui->userNameLineEdit->setText(dbCredentials.at(2));
        ui->passwordLineEdit->setText(dbCredentials.at(3));
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

void Parametres::on_searchButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this,tr("Choisissez un dossier contenant des fichiers TDMS"), directory,QFileDialog::DontUseNativeDialog);
    ui->tdmsFileLineEdit->setText(directory);
}

void Parametres::on_saveButton_clicked()
{
    QVariantList credentialsDb;
    credentialsDb << ui->domainNameLineEdit->text() << ui->databaseNameLineEdit->text() << ui->userNameLineEdit->text();
    if(ui->passwordLineEdit->text().isEmpty()) {
        credentialsDb << "";
    }
    else {
        credentialsDb << ui->passwordLineEdit->text();
    }
    writeRegister(directory,credentialsDb);
    close();
}

void Parametres::on_cancelButton_clicked()
{
    close();
}

void Parametres::testConnexion() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","testMysql");
    db.setHostName(ui->domainNameLineEdit->text());
    db.setDatabaseName(ui->databaseNameLineEdit->text());
    db.setUserName(ui->userNameLineEdit->text());
    db.setPassword(ui->passwordLineEdit->text());
    if (db.open()) {
        QMessageBox::information(this,"Test de la connexion à la base de donnée","Connexion réussie :)");
    }
    else {
        QMessageBox::critical(this,"Test de la connexion à la base de donnée","Echec de la connexion (Veuillez vérifier vos identifiants).");
    }
    db.close();
    db.removeDatabase("testMysql");
}

void Parametres::on_testConnexionButton_clicked()
{
    testConnexion();
}
