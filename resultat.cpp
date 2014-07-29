#include "resultat.h"
#include "ui_resultat.h"

Resultat::Resultat(QString fichierManchot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Resultat)
{
    ui->setupUi(this);

    histoPlot  = new QwtPlot(this);
    histoPlot->setTitle("Histogramme Erreurs");

    curvePlot = new QwtPlot(this);
    curvePlot->setTitle("Evolution de la Masse");

    curve= new QwtPlotCurve();
    histo = new QwtPlotHistogram("Histo");
    curveT= new QwtPlotMarker();

    ui->horizontalLayout->addWidget(histoPlot);
    ui->horizontalLayout->addWidget(curvePlot);

    fileName = fichierManchot;

    histoPlot->setVisible(false);
    curvePlot->setVisible(true);

    caractManchot(-1, fileName);
}

Resultat::~Resultat()
{
    delete ui;
}

void Resultat::caractManchot(double poidsTheo, QString fichierManchot) {
    int i=0,nb_val=0,nb_passage=0,numValidated=0,tailleMax = 50;
    double k=0,max=0,min=0;
    double** dataCourbe,*smoothData;
    char cas;
    bool caseTab[2]={true,true};
    QString dateTime;
    double moy=0,var=0,errorTot=0;
    QVector<double> Error(2000,0.0);
    double* masse[2];
    masse[0] = (double*)calloc(50,sizeof(double));
    masse[1] = (double*)calloc(50,sizeof(double));

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

            masse[0][numValidated] = numValidated;
            masse[1][numValidated] = caractCourbe[0];

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

                 masse[0][numValidated] = numValidated;
                 masse[1][numValidated] = caractSelected[0];


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

        if( numValidated > (tailleMax-2) ) {
            tailleMax += 50;
            masse[0]= (double*)realloc(masse[0],tailleMax * sizeof(double));
            masse[1]= (double*)realloc(masse[1],tailleMax * sizeof(double));
        }
    }
    moy = moy/k;
    var = var/k - (moy*moy);
    errorTot =  100*errorTot/k;

    ui->label->setText("Masse moyenne: "+QString::number(moy));
    ui->label_6->setText("Nombre de courbes traitées: "+QString::number(numValidated)+"/" + QString::number(i));
    ui->label_3->setText("Ecart-type: "+QString::number(sqrt(var)));

    if ( poidsTheo != -1 ) {
        ui->label_2->setText("Erreur totale: "+QString::number(errorTot)+" %");

        QVector<QwtIntervalSample> errorSample(2000);

        int fin = floor(1000*max);
        int debut = floor(1000*min);

        for(i = debut ;i<fin;i++) {
            errorSample.append(QwtIntervalSample(Error[i+1000]/k, double(i/10.0) , double(i/10.0) + 0.1 ));
        }

        histo->setData(new QwtIntervalSeriesData(errorSample));
        histo->attach(histoPlot);

        histoPlot->updateAxes();
        histoPlot->replot();
        if ( histoPlot->isVisible() ) {
            histoPlot->show();
        }



        curveT->setYValue(ui->lineEdit->text().toDouble());
        curveT->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
        curveT->setLineStyle(QwtPlotMarker::HLine);
        curveT->setLinePen(Qt::green);

        curveT->attach(curvePlot);
    }
    if (numValidated > 0 ) {
        curve->setRawSamples(masse[0],masse[1],numValidated);
        curve->attach(curvePlot);
    }

    Error.clear();

    curvePlot->updateAxes();
    curvePlot->replot();
    if ( curvePlot->isVisible() ) {
        curvePlot->show();
    }
}

void Resultat::on_pushButton_clicked()
{
    QTimer *timer = new QTimer;
    if ( !ui->lineEdit->text().isEmpty() ) {
        caractManchot(ui->lineEdit->text().toDouble(), fileName);
    }
    else {
        caractManchot(-1, fileName);
    }
    timer->start(500);
}

void Resultat::on_pushButton_2_clicked()
{
    histoPlot->setVisible(false);
    curvePlot->setVisible(true);
}

void Resultat::on_pushButton_3_clicked()
{
    histoPlot->setVisible(true);
    curvePlot->setVisible(false);
}
