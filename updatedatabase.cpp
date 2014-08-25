#include "updatedatabase.h"
#include "ui_updatedatabase.h"

UpdateDatabase::UpdateDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDatabase)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->hide();

    connect(&this->FutureWatcher,SIGNAL(finished()),this,SLOT(slot_finished()));
}

UpdateDatabase::~UpdateDatabase()
{
    delete ui;
}

void UpdateDatabase::on_submitButton_clicked()
{
    QString dest = QDir::homePath() + "/.ihmanchots";
    QString directory = ReadRegister::readInRegister("OpenTDMSDialog/destinationPath").toString();

    if (ui->periodTab->isVisible()) {
        ui->progressBar->show();
        QFuture<void> future = QtConcurrent::run(&this->ot,&OpenTdms::UpadateAllManchot,directory,dest,ui->startDateEdit->date(),ui->endDateEdit->date());
        this->FutureWatcher.setFuture(future);
    }
    else {
        int idManchot = ui->spinBox->value();
        int nbResult;
        QString filenameRead;
        nbResult = ot.ManchotTxt(directory,dest,idManchot);
        filenameRead = dest+"/Temp/Manchot"+QString::number(idManchot)+".txt";
        //caractManchot(filenameRead,true);
        close();
    }

}

void UpdateDatabase::on_cancelButton_clicked()
{
    close();
}

void UpdateDatabase::slot_finished() {
    ui->progressBar->hide();
    close();
}



