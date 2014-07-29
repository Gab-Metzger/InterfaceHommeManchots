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

    init = true;
    caractManchot(-1, fileName);
    init = false;
}

Resultat::~Resultat()
{
    delete ui;
}

int Resultat::compar(QString arg1,QString arg2,int nb) {
    int information[2][7];
    int res=1,i=0;

    sscanf(qPrintable(arg1),"%d-%d-%d %d-%d-%d-%d\n",&information[0][0],&information[0][1],&information[0][2],&information[0][3],&information[0][4],&information[0][5],&information[0][6]);
    sscanf(qPrintable(arg2),"%d-%d-%d %d-%d-%d-%d\n",&information[1][0],&information[1][1],&information[1][2],&information[1][3],&information[1][4],&information[1][5],&information[1][6]);

    while ( ( i < (3 + nb*4) ) && (information[0][i] == information[1][i]) ) {
        i++;
    }
    if ( i == (3 + nb*4)  ) {
        res = 0;
    }
    else if ( information[0][i] < information[1][i] ) {
        res = -1;
    }

    return res;
}

void Resultat::tri_Bulle(QStringList *arg,double **masse) {
    int i=0,l=0,n=arg->length();
    while( i < n ) {
        while ( (l < (n-i-1) ) && ( compar((*arg)[i+l],(*arg)[i+l+1],1) > 0 ) ) {
            QString tmp = (*arg)[i+l];
            double tmpD = masse[1][i+l+1];

            (*arg)[i+l] = (*arg)[i+l+1];
            (*arg)[i+l+1] = tmp;

            masse[1][i+l+1] = masse[1][i+l];
            masse[1][i+l] = tmpD;

            l++;
        }
        i++;
        l=0;
    }
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
    QStringList TabDate,comboList;
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

            TabDate << dateTime.mid(0,dateTime.length()-4);

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

                 TabDate << dateTime.mid(0,dateTime.length()-4);

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


    tri_Bulle(&TabDate,masse);
    if ( init ) {
        comboList << TabDate[0].mid(0,10);
        int l=1;
        for(int i =0;i<TabDate.length();i++) {
            if ( compar(TabDate[i],comboList[l-1],0) != 0 ) {
                comboList << TabDate[i].mid(0,10);
                l++;
            }
        }

        ui->comboBox->addItems(comboList);
        ui->comboBox_2->addItems(comboList);
        ui->comboBox_2->setCurrentIndex(l-1);
    }

    moy = moy/k;
    var = var/k - (moy*moy);
    errorTot =  100*errorTot/k;

    ui->meanWeightLabel->setText("Masse moyenne: "+QString::number(moy));
    ui->nbCurveLabel->setText("Nombre de courbes traitées: "+QString::number(numValidated)+"/" + QString::number(i));
    ui->deviationLabel->setText("Ecart-type: "+QString::number(sqrt(var)));

    if ( poidsTheo != -1 ) {
        ui->totalErrorLabel->setText("Erreur totale: "+QString::number(errorTot)+" %");

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

        curveT->setYValue(ui->theoLineEdit->text().toDouble());
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

void Resultat::on_theoValidateButton_clicked()
{
    QTimer *timer = new QTimer;
    if ( !ui->theoLineEdit->text().isEmpty() ) {
        caractManchot(ui->theoLineEdit->text().toDouble(), fileName);
    }
    else {
        caractManchot(-1, fileName);
    }
    timer->start(500);
}

void Resultat::on_weightGraphButton_clicked()
{
    histoPlot->setVisible(false);
    curvePlot->setVisible(true);
}

void Resultat::on_histoGraphButton_clicked()
{
    histoPlot->setVisible(true);
    curvePlot->setVisible(false);
}

void Resultat::on_comboBox_2_currentIndexChanged(int index)
{
    if ( index < ui->comboBox->currentIndex() ) {
        ui->comboBox_2->setCurrentIndex(ui->comboBox->currentIndex());
    }
}

void Resultat::on_comboBox_currentIndexChanged(int index)
{
    if ( index > ui->comboBox_2->currentIndex()) {
        ui->comboBox_2->setCurrentIndex(index);
    }
}
