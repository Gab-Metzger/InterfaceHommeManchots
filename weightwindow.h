#ifndef WEIGHTWINDOW_H
#define WEIGHTWINDOW_H

#include <QDialog>

namespace Ui {
class WeightWindow;
}

class WeightWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WeightWindow(QWidget *parent = 0);
    ~WeightWindow();

private:
    Ui::WeightWindow *ui;
};

#endif // WEIGHTWINDOW_H
