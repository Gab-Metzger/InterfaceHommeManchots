#ifndef ALGOPLATMANAGER_H
#define ALGOPLATMANAGER_H

#include <QDialog>
#include <QSettings>
#include "readregister.h"

namespace Ui {
class AlgoPlatManager;
}

class AlgoPlatManager : public QDialog
{
    Q_OBJECT

public:
    explicit AlgoPlatManager(QWidget *parent = 0);
    ~AlgoPlatManager();
    //Getters
    int getHorizontalThreshold();
    double getVerticalThreshold();
    double getLevelThreshold();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AlgoPlatManager *ui;
    int horizontalThreshold;
    double verticalThreshold;
    double levelThreshold;
    void writeRegister();
};

#endif // ALGOPLATMANAGER_H
