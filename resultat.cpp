#include "resultat.h"
#include "ui_resultat.h"

Resultat::Resultat(QString fichierManchot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Resultat)
{
    ui->setupUi(this);
    filename = fichierManchot;
}

Resultat::~Resultat()
{
    delete ui;
}

void Resultat::caractManchot(double poidsTheo, QString fichierManchot) {
    int i=0,nb_val=0,nb_passage=0,numValidated=0;
    double k=0,max=0,min=0;
    double** dataCourbe,*smoothData;
    char cas;
    bool caseTab[2]={true,true};
    QString dateTime;
    double moy=0,var=0,errorTot=0;
    QVector<double> Error(2000,0.0);
    Stat* statArray = new Stat();
    Flat* indexArray = new Flat();


    while (lect.lire_fichier(fichierManchot,&dataCourbe,&nb_val,&nb_passage,&cas,i,&dateTime,caseTab) != 2 ) {
        i++;
        for(int l=0;l< 4;l++) {
            smoothData=(double*)calloc(nb_val,sizeof(double));
            for(int k=0;k < nb_val;k++) {
                if ((k == 0) || (k==(nb_val-1))) {
                    smoothData[k] = dataCourbe[l][k];
                }
                else {
                    smoothData[k] = (dataCourbe[l][k-1] + dataCourbe[l][k] + dataCourbe[l][k+1])/3;
                }
            }
            memccpy(dataCourbe[l],smoothData,nb_val,sizeof(double));
            free(smoothData);
        }

        QVector<double> caractCourbe = anal.getWeightByFlat(dataCourbe,nb_val,indexArray,statArray);

        if (  anal.isGoodFlat(caractCourbe, dataCourbe,nb_val) ) {
            double error = (caractCourbe[0] - poidsTheo) / poidsTheo;

            moy += caractCourbe[1]*caractCourbe[0];
            var += caractCourbe[1]*caractCourbe[0]*caractCourbe[0];


            Error[floor(1000*(error + 1) )] += caractCourbe[1];
            errorTot += caractCourbe[1]*error;

            k+=caractCourbe[1];
            numValidated++;

            if ( error > max ) {max = error;}
            if ( error < min ) {min = error;}
        }
        else {
             histoDialog *diag = new histoDialog();
             double maxData[4];

             diag->calculMax(dataCourbe,nb_val,maxData);
             QVector<double> caractSelected(2);
             caractSelected = diag->caractLoiNormal(maxData,dataCourbe,nb_val);

             if ( caractSelected[0] > 0 ) {
                 double error = (caractSelected[0] - poidsTheo) / poidsTheo;

                 moy += caractSelected[1]*caractSelected[0];
                 var += caractSelected[1]*caractSelected[0]*caractSelected[0];

                 Error[floor(1000*(error+1))] += caractSelected[1];
                 errorTot += caractSelected[1]*error;

                 k+=caractSelected[1];
                 numValidated++;

                 if ( error > max ) {max = error;}
                 if ( error < min ) {min = error;}
             }
        }
    }
    moy = moy/k;
    var = var/k - moy*moy;
    errorTot =  100*errorTot/k;

    ui->label->setText(QString(ui->label->text()+QString::number(moy)));
    ui->label_3->setText(QString(ui->label_3->text()+QString::number(sqrt(var))));
    ui->label_2->setText(QString(ui->label_2->text()+QString::number(errorTot)+" %"));
    ui->label_6->setText(QString(ui->label_6->text()+QString::number(numValidated)+"/" + QString::number(i)));

    histoPlot  = new QwtPlot(this);
    histoPlot->setTitle("Histogramme 2");
    histo = new QwtPlotHistogram("Histo");
    ui->horizontalLayout->addWidget(histoPlot);

    histo->setYAxis( QwtPlot::yLeft );
    histo->setXAxis( QwtPlot::xBottom );
    histo->attach(histoPlot);

    QVector<QwtIntervalSample> errorSample(2000);

    int fin = floor(1000*max);
    int debut = floor(1000*min);

    for(i = debut ;i<fin;i++) {
        errorSample.append(QwtIntervalSample(Error[i+1000]/k, double(i/10.0) , double(i/10.0) + 0.1 ));
    }

    histo->setData(new QwtIntervalSeriesData(errorSample));
    Error.clear();

    histoPlot->show();
}

void Resultat::on_pushButton_clicked()
{
    caractManchot(ui->lineEdit->text().toDouble(), filename);
}
