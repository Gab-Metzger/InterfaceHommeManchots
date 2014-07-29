#ifndef OPENTDMSDIALOG_H
#define OPENTDMSDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QAbstractButton>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QFutureWatcher>
#include <QProgressDialog>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "opentdms.h"

namespace Ui {
class openTDMSDialog;
}

class openTDMSDialog : public QDialog
{
    Q_OBJECT

public:
    explicit openTDMSDialog(QWidget *parent = 0);
    ~openTDMSDialog();
    /**
     * @brief getFileName
     * @return
     */
    QString getFileName();
    bool getAuthorizationDraw();

public slots:
    void slot_finished();

private slots:
    void on_submitButton_clicked();

    void on_calendarWidget_selectionChanged();

private:
    Ui::openTDMSDialog *ui;
    QString directory;
    QString filenameRead;
    QString readRegister();
    QFutureWatcher<void> futureWatcher;
    QProgressDialog* progressDialog;
    OpenTdms files;
    bool authorization;
    /**
     * @brief writeRegister
     * @param path
     */
    void writeRegister();
};

#endif // OPENTDMSDIALOG_H
