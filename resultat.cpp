#include "resultat.h"
#include "ui_resultat.h"

Resultat::Resultat(QString fichierManchot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Resultat)
{
    ui->setupUi(this);

    tmpX=NULL;
    tmpY=NULL;

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

    caractManchot(fileName);
}

Resultat::~Resultat()
{
    free(masse[0]);
    free(masse[1]);
    free(masse[2]);
    free(tmpX);
    free(tmpY);
    delete ui;
}

statInfo Resultat::initStatistique(int n) {
    statInfo statistique;
    statistique.dataX = (double*)calloc(n,sizeof(double));
    statistique.dataY = (double*)calloc(n,sizeof(double));
    statistique.nbVal = 0;
    statistique.moy = 0;
    statistique.var = 0;
    statistique.size = 0;
    return statistique;
}

int convertion(QString arg,int i) {
    char xc = arg[i].toLatin1();
    int x=atoi(&xc);
    if ( arg[i-1] == '-' ) {
        x = 10 * x;
        if ( arg[i+1] == '-' ) {
            x = x /10;
        }
    }
    return x;
}

int Resultat::compar(QString arg1,QString arg2,int nb) {
    int res=1,i=0;

    while ( ( i < (10 + 13*nb) ) && ( arg1[i] == arg2[i]) ){
        i++;
    }

    if ( i == (10 + 13*nb)  ) {
        res = 0;
    }
    else {
        int x1 = 0,x2=0;
        x1 = convertion(arg1,i);
        x2 = convertion(arg2,i);

        if ( x1 < x2 ){
            res = -1;
        }
    }

    return res;
}

void Resultat::tri_Bulle(QStringList *arg,double **masse) {
    int n=arg->length();
    bool echange;

    do {
        echange = false;
        for(int i = 0;i <(n-1);i++) {
            if ( compar((*arg)[i],(*arg)[i+1],1) > 0 ) {
                QString tmp = (*arg)[i];
                double tmpD = masse[1][i+1];

                (*arg)[i] = (*arg)[i+1];
                (*arg)[i+1] = tmp;

                masse[1][i+1] = masse[1][i];
                masse[1][i] = tmpD;

                echange = true;
            }
        }
        n--;

    } while( echange );
}

void Resultat::caractManchot(QString fichierManchot) {
    int i=0,nbVal=0,nb_passage=0,tailleMax = 50;
    double** dataCourbe;
    char cas;
    bool caseTab[2]={true,true};
    QString dateTime;

    QStringList TabDate;
    statInfo statistique=initStatistique(0);

    masse[0] = (double*)calloc(50,sizeof(double));
    masse[1] = (double*)calloc(50,sizeof(double));
    masse[2] = (double*)calloc(50,sizeof(double));

    Stat* statArray = new Stat();
    Flat* indexArray = new Flat();

    while (lect.lire_fichier(fichierManchot,&dataCourbe,&nbVal,&nb_passage,&cas,i,&dateTime,caseTab) != 2 ) {
        i++;
        lissageCourbe(dataCourbe,nbVal);

        QVector<double> caractCourbe = anal.getWeightByFlat(dataCourbe,nbVal,indexArray,statArray);

        if (  anal.isGoodFlat(caractCourbe, dataCourbe,nbVal) ) {

            masse[0][statistique.nbVal] = statistique.nbVal;
            masse[1][statistique.nbVal] = caractCourbe[0];
            masse[2][statistique.nbVal] = caractCourbe[1];

            TabDate << dateTime.mid(0,dateTime.length()-4);

            miseAJourCaract(&statistique,statistique.nbVal);
        }
        else {
             histoDialog *diag = new histoDialog();
             double maxData[4];

             diag->calculMax(dataCourbe,nbVal,maxData);
             QVector<double> caractSelected(2);
             caractSelected = diag->caractLoiNormal(maxData,dataCourbe,nbVal);

             if ( caractSelected[0] > 0 ) {

                 masse[0][statistique.nbVal] = statistique.nbVal;
                 masse[1][statistique.nbVal] = caractSelected[0];
                 masse[2][statistique.nbVal] = caractSelected[1];

                 TabDate << dateTime.mid(0,dateTime.length()-4);

                 miseAJourCaract(&statistique,statistique.nbVal);
             }
        }

        if( statistique.nbVal > (tailleMax-2) ) {
            tailleMax += 50;
            masse[0]= (double*)realloc(masse[0],tailleMax * sizeof(double));
            masse[1]= (double*)realloc(masse[1],tailleMax * sizeof(double));
            masse[2]= (double*)realloc(masse[2],tailleMax * sizeof(double));
        }
    }
    numValidated = statistique.nbVal;
    initComboBox(TabDate);
    statistique.dataX=masse[0];
    statistique.dataY=masse[1];
    nbPassage = i;

    traceCourbe(statistique);
}

void Resultat::miseAJourCaract(statInfo *statistique,int numValidated) {

    statistique->moy += masse[2][numValidated] * masse[1][numValidated];
    statistique->var += masse[2][numValidated] * masse[1][numValidated] * masse[1][numValidated];

    statistique->size += masse[2][numValidated];
    statistique->nbVal += 1;
}


void Resultat::lissageCourbe(double** dataCourbe,int nbVal){
    double *smoothData;

    for(int l=0;l< 4;l++) {
        smoothData=(double*)calloc(nbVal,sizeof(double));
        for(int k=0;k < nbVal;k++) {
            if ((k == 0) || (k==(nbVal-1))) {
                smoothData[k] = dataCourbe[l][k];
            }
            else {
                smoothData[k] = (dataCourbe[l][k-1] + dataCourbe[l][k] + dataCourbe[l][k+1])/3;
            }
        }
        memccpy(dataCourbe[l],smoothData,nbVal,sizeof(double));
        free(smoothData);
    }
}

void Resultat::initComboBox(QStringList TabDate) {
    QStringList comboList;

    tri_Bulle(&TabDate,masse);
    comboList << TabDate[0].mid(0,10);

    int l=1;
    for(int i =0;i<numValidated;i++) {
        if ( compar(TabDate[i],comboList[l-1],0) != 0 ) {
            comboList << TabDate[i].mid(0,10);
            l++;
        }
        dateInfo << TabDate[i].mid(0,10);
    }

    ui->comboBox->addItems(comboList);
    ui->comboBox_2->addItems(comboList);
    ui->comboBox_2->setCurrentIndex(l-1);
}

void Resultat::traceCourbe(statInfo statistique){

    statistique.moy = statistique.moy/statistique.size;
    statistique.var = statistique.var/statistique.size - (statistique.moy*statistique.moy);

    ui->meanWeightLabel->setText("Masse moyenne: "+QString::number(statistique.moy));
    ui->nbCurveLabel->setText("Nombre de courbes traitées: "+QString::number(statistique.nbVal)+"/" + QString::number(nbPassage));
    ui->deviationLabel->setText("Ecart-type: "+QString::number(sqrt(statistique.var)));

    if (statistique.nbVal > 0 ) {
        curve->setRawSamples(statistique.dataX,statistique.dataY,statistique.nbVal);
        curve->attach(curvePlot);
    }

    curvePlot->updateAxes();
    curvePlot->replot();
    if ( curvePlot->isVisible() ) {
        curvePlot->show();
    }
}

void Resultat::initHistoMasse(double poidsTheo, histoMasseInfo *info,QVector< QStringList > periode) {
    int nbImpossible=0;
    for(int i=0;i< numValidated;i++) {
        if ( ( compar(periode[0][0],periode[1][i],0) <= 0 ) && ( compar(periode[0][1],periode[1][i],0) >= 0 )  ) {

            info->statistique.dataX[info->statistique.nbVal] = info->statistique.nbVal;
            info->statistique.dataY[info->statistique.nbVal] = masse[1][i];
            miseAJourCaract(&info->statistique,i);
            if ( poidsTheo != -1 ) {
                info->error = (masse[1][i] - poidsTheo) / poidsTheo;
                if ( info->error < 1 ) {

                    info->Error[floor(1000*( info->error + 1) )] += masse[2][i];
                    info->errorTot += masse[2][i] * info->error;

                    if ( info->error > info->max ) {info->max = info->error;}
                    if ( info->error < info->min ) {info->min = info->error;}
                }
                else {
                    nbImpossible++;
                }
            }
            info->k += masse[2][i];
        }
    }
    if ( nbImpossible > 0 ) {
        if ( nbImpossible == numValidated ){
            ui->label->setText(QString("Attention Histogramme vide\n%1 éléments plus de 100% d'erreur").arg(nbImpossible));
            ui->label->setVisible(true);
        }
        else {
            ui->label->setText(QString("Attention Histogramme incomplet\n%1 éléments plus de 100% d'erreur").arg(nbImpossible));
            ui->label->setVisible(true);
        }
    }
    else {
        ui->label->setVisible(false);
    }
}

histoMasseInfo Resultat::initInfo(statInfo statistique){
    histoMasseInfo info;

    info.min=100;
    info.max=-100;
    info.k=0;
    info.error=0;
    info.errorTot=0;
    info.statistique = statistique;
    info.Error = *new QVector<double>(2000,0.0);
    return info;
}

void Resultat::traceHistoMasse(double poidsTheo,QStringList periode1) {

    statInfo statistique = initStatistique(numValidated);
    histoMasseInfo info = initInfo(statistique);
    qDebug() << "ici";
    if ( tmpX != NULL ) {
        free(tmpX);
        tmpX = statistique.dataX;
    }
    if ( tmpY != NULL ) {
        free(tmpY);
        tmpY = statistique.dataY;
    }
    qDebug() << "là";
    QVector<QStringList> periode(2);
    periode[0] = periode1;
    periode[1] = dateInfo;

    initHistoMasse(poidsTheo,&info,periode);

    info.errorTot = 100*info.errorTot/info.k;

    ui->totalErrorLabel->setText("Erreur totale: "+QString::number(info.errorTot)+" %");

    QVector<QwtIntervalSample> errorSample(2000);

    int fin = floor(1000*info.max);
    int debut = floor(1000*info.min);

    for(int i = debut ;i<fin;i++) {
        errorSample.append(QwtIntervalSample(info.Error[i+1000]/info.k, double(i/10.0) , double(i/10.0) + 0.1 ));
    }

    if ( poidsTheo != -1 ) {
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

        curvePlot->replot();
    }

    info.statistique.size=info.k;
    traceCourbe(info.statistique);
    info.Error.clear();
}

void Resultat::on_theoValidateButton_clicked()
{
    QTimer *timer = new QTimer;
    miseAJourPeriode();
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
    miseAJourPeriode();
}

void Resultat::on_comboBox_currentIndexChanged(int index)
{
    if ( index > ui->comboBox_2->currentIndex()) {
        ui->comboBox_2->setCurrentIndex(index);
    }
    miseAJourPeriode();
}

void Resultat::miseAJourPeriode() {
    QStringList periode;
    periode << ui->comboBox->currentText() << ui->comboBox_2->currentText();
    if ( !ui->theoLineEdit->text().isEmpty() ) {            
        traceHistoMasse(ui->theoLineEdit->text().toDouble(),periode);
    }
    else {
        traceHistoMasse(-1,periode);
    }
}

