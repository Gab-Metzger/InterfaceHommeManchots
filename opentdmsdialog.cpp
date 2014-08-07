#include "opentdmsdialog.h"
#include "ui_opentdmsdialog.h"

openTDMSDialog::openTDMSDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openTDMSDialog)
{
    ui->setupUi(this);
    directory = readRegister();  
    ui->tdmsFileLabel->setText(QString("Répertoire sélectionné : " + directory));

    QDir calendarDayExist(directory);
    ui->isTdmsLabel->clear();
    if(calendarDayExist.exists(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"))) {
         ui->isTdmsLabel->setText("TDMS présent");
         ui->isTdmsLabel->setStyleSheet("QLabel {color: green;}");
    }
    else {
        ui->isTdmsLabel->setText("TDMS introuvable");
        ui->isTdmsLabel->setStyleSheet("QLabel {color: red;}");
    }

    this->progressDialog = new QProgressDialog();
    connect(&this->futureWatcher,SIGNAL(finished()),this,SLOT(slot_finished()));
    connect(&this->futureWatcher,SIGNAL(finished()),this->progressDialog,SLOT(cancel()));

    //Can we draw the curve ?
    authorization = false;
}

openTDMSDialog::~openTDMSDialog()
{
    writeRegister();
    delete ui;
}

QString openTDMSDialog::getFileName() {
    return filenameRead;
}

void openTDMSDialog::on_submitButton_clicked()
{    
    QString balanceChoice, date;
    QString hour;
    QString dest = QDir::homePath() + "/.ihmanchots";
    int error;

    date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    hour = QString::number(ui->spinBox->value());
    if(hour.length() < 2) {
        hour = "0"+hour;
    }

    if(ui->plate123RadioButton->isChecked()) {
        balanceChoice = ui->plate123RadioButton->text();
    }
    else if (ui->plate456RadioButton->isChecked()) {
        balanceChoice = ui->plate456RadioButton->text();
    }
    else{
        balanceChoice = ui->plate789RadioButton->text();
    }
    filenameRead = dest + "/Temp/" + hour + "-00-00 plateau" + balanceChoice + ".txt";
    QDir erreur(QString(dest+"/Temp"));
    erreur.removeRecursively();
    QString tdmsname = hour + "-00-00 plateau" + balanceChoice + ".txt";
    QString infoFile = date + " " + hour + " " + balanceChoice;
    QFuture<int> future = QtConcurrent::run(&this->files,&OpenTdms::creation_txt,directory,dest,infoFile,-1);
    this->futureWatcher.setFuture(future);
    error = future.result();
    qDebug() << error;
    if (error == -3) {
        QMessageBox::critical(this,"Erreur","Le dossier spécifié ne contient pas de fichier TDMS");
    }else {
        if (erreur.exists(tdmsname)) {
            this->progressDialog->setMinimum(0);
            this->progressDialog->setMaximum(0);
            this->progressDialog->setWindowModality(Qt::WindowModal);
            this->progressDialog->exec();
            authorization = true;
            close();
        }else {
            QMessageBox::critical(this,"Erreur","N/A : Il n'y a pas de données à cette heure-ci.");
        }

    }

}

QString openTDMSDialog::readRegister() {
    QString destination;
    QSettings settings("METZGER","IHManchots");

    destination = settings.value("OpenTDMSDialog/destinationPath").toString();
    ui->calendarWidget->setSelectedDate(settings.value("OpenTDMSDialog/selectedDate").toDate());
    return destination;
}

void openTDMSDialog::writeRegister() {
    QSettings settings("METZGER","IHManchots");
    settings.setValue("OpenTDMSDialog/selectedDate",ui->calendarWidget->selectedDate());
}

void openTDMSDialog::slot_finished() {
    std::cout << "Finished\n";
}

void openTDMSDialog::on_calendarWidget_selectionChanged()
{
   QDir calendarDayExist(directory);
   ui->isTdmsLabel->clear();
   if(calendarDayExist.exists(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"))) {
        ui->isTdmsLabel->setText("TDMS présent");
        ui->isTdmsLabel->setStyleSheet("QLabel {color: green;}");
   }
   else {
       ui->isTdmsLabel->setText("TDMS introuvable");
       ui->isTdmsLabel->setStyleSheet("QLabel {color: red;}");
   }
}

bool openTDMSDialog::getAuthorizationDraw() {
    return authorization;
}
