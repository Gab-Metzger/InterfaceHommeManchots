#include "algoplatmanager.h"
#include "ui_algoplatmanager.h"

AlgoPlatManager::AlgoPlatManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlgoPlatManager)
{
    ui->setupUi(this);
    horizontalThreshold = ReadRegister::readInRegister("AlgoPlatManager/horizontalThreshold").toInt();
    verticalThreshold = ReadRegister::readInRegister("AlgoPlatManager/verticalThreshold").toDouble();
    levelThreshold = ReadRegister::readInRegister("AlgoPlatManager/levelThreshold").toDouble();
    ui->horizontalSpinBox->setValue(horizontalThreshold);
    ui->verticalDoubleSpinBox->setValue(verticalThreshold);
    ui->levelDoubleSpinBox->setValue(levelThreshold);
}

AlgoPlatManager::~AlgoPlatManager()
{
    delete ui;
}

int AlgoPlatManager::getHorizontalThreshold() { return horizontalThreshold; }
double AlgoPlatManager::getVerticalThreshold() { return verticalThreshold; }
double AlgoPlatManager::getLevelThreshold() { return levelThreshold; }


void AlgoPlatManager::on_submitButton_clicked()
{
    //Updates threshold values on register
    writeRegister();
    close();
}

void AlgoPlatManager::on_cancelButton_clicked()
{
    close();
}

void AlgoPlatManager::writeRegister() {
    QSettings settings("METZGER","IHManchots");
    settings.setValue("AlgoPlatManager/horizontalThreshold",ui->horizontalSpinBox->value());
    settings.setValue("AlgoPlatManager/verticalThreshold",ui->verticalDoubleSpinBox->value());
    settings.setValue("AlgoPlatManager/levelThreshold",ui->levelDoubleSpinBox->value());
}
