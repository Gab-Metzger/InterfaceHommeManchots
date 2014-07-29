#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir dir(QString(QDir::homePath()+"/.ihmanchots/Temp"));
    if ( dir.exists() ) {
        dir.removeRecursively();
    }

    ui->setupUi(this);
    readSettings();
    plot  = new QwtPlot(this);
    plot->setTitle("Courbe poids");
    plot->setAxisTitle(QwtPlot::xBottom,"millisecondes");
    plot->setAxisTitle(QwtPlot::yLeft,"poids (kg)");
    plot->setCanvasBackground(* new QBrush(Qt::white));

    for(int j = 0 ;j<4;j++) {
        curve[j]= new QwtPlotCurve();
    }
    ui->plotLayout->addWidget(plot);
    ui->actionAlgorithme_de_plats->setDisabled(true);
    ui->actionAfficher->setDisabled(true);
    ui->spinBox->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton->setDisabled(true);
    ui->label_3->setStyleSheet("QCheckBox {color: red;}");
    ui->balance1->setStyleSheet("QCheckBox {color: blue;}");
    ui->balance2->setStyleSheet("QCheckBox {color: green;}");
    ui->balance3->setStyleSheet("QCheckBox {color: red;}");
    ui->actionR_sultat->setDisabled(true);

    existFlat = false;
    omWindow = new OuvertureManchot(this);
}
MainWindow::~MainWindow()
{
    QDir dir(QString(QDir::homePath()+"/.ihmanchots/Temp"));
    dir.removeRecursively();

    writeSettings();
    delete omWindow;
    delete ui;
}


void MainWindow::on_actionOuvrir_triggered()
{
    ui->actionR_sultat->setDisabled(true);
    ui->spinBox->setValue(0);
    openTDMSDialog ot;
    ot.setAttribute(Qt::WA_QuitOnClose);
    ot.exec();
    filename = ot.getFileName();
    if (ot.getAuthorizationDraw()) {
        tracer(0,0,-1);
    }
    this->setWindowTitle(filename);
}

void MainWindow::on_actionQuitter_triggered()
{
  QApplication::quit();
}

void MainWindow::tracer(int num_passage, int minInt, int maxInt, int shouldSmooth) {

    char titre[100];
    double* xs;
    double **dataInterval;
    QString dateTime="";
    int h,m,s,ms,max;
    analyse ANALYSE;
    Flat *indexArray = new Flat();
    Stat *statArray =  new Stat();
    QVector<double> weightWithConfidence(2);
    QString output = "";
    bool caseTab[2];

    caseTab[0] = ui->checkBox_2->isChecked();
    caseTab[1] = ui->checkBox->isChecked();

    errorOpenFile = FICHIERS.lire_fichier(filename,&l_data,&nb_valeur,&nb_passage,&cas,num_passage,&dateTime,caseTab);
    if (errorOpenFile != 1 ) {
        if (maxInt == -1) {
            plot->setAxisScale(QwtPlot::xBottom,minInt,nb_valeur);
            ui->spinBox_2->setMaximum(nb_valeur);
            ui->spinBox_3->setMaximum(nb_valeur);
            ui->spinBox_3->setValue(nb_valeur);
        }
        else {
            plot->setAxisScale(QwtPlot::xBottom,minInt,maxInt);
            ui->spinBox_2->setMaximum(maxInt);
            ui->spinBox_3->setMaximum(maxInt);
            ui->spinBox_3->setValue(maxInt); 
        }

        ui->spinBox->setRange(0,nb_passage-1);


        ui->balance1->setChecked(1);
        ui->balance2->setChecked(1);
        ui->balance3->setChecked(1);
        ui->sommebalance->setChecked(0);
        ui->actionAlgorithme_de_plats->setEnabled(true);
        ui->actionAfficher->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton->setEnabled(true);

        selection[0] = 1;
        selection[1] = 1;
        selection[2] = 1;
        selection[3] = 0;

        for(int j = 0 ;j<4;j++) {
            curve[j]->detach();
        }

        char date[13];
        sscanf(qPrintable(dateTime),"%s %d-%d-%d-%d",date,&h,&m,&s,&ms);

        if ( cas == 'c' ) {
           sprintf(titre,"%s\n%d h %d m %d s %d ms\nCas complexe",date,h,m,s,ms);
        }
        else{
           sprintf(titre,"%s\n%d h %d m %d s %d ms\nCas simple",date,h,m,s,ms);
        }

        plot->setTitle(titre);

        if(maxInt == -1) {
            max = nb_valeur;
        }
        else {
            max = maxInt;
        }

        xs =(double*)malloc((max-minInt)*sizeof(double));
        dataInterval = (double**)calloc(4,sizeof(double*));

        for (int i=0;i<4;i++) {
            dataInterval[i] = (double*)calloc((max-minInt),sizeof(double));
            for(int j=minInt;j<max;j++) {
                if (shouldSmooth > 0) {
                    if ((j == 0) || (j==(nb_valeur-1))) {
                        dataInterval[i][j-minInt] = l_data[i][j];
                    }
                    else {
                        dataInterval[i][j-minInt] = (l_data[i][j-1] + l_data[i][j] + l_data[i][j+1])/3;
                    }
                }
                else {
                    dataInterval[i][j-minInt] = l_data[i][j];
                }

            }
        }

        if (shouldSmooth > 0) {
            for (int i=0;i<4;i++) {
                for(int j=minInt;j<max;j++) {
                    l_data[i][j] = dataInterval[i][j-minInt];
                }
            }
        }

        for (int x = minInt; x < max; x++) {
            xs[x-minInt]=x;
        }

        color[0] = new QPen(Qt::blue);
        color[1] = new QPen(Qt::green);
        color[2] = new QPen(Qt::red);
        color[3] = new QPen(Qt::black);
        QwtPlotZoomer* zoomer = new QwtPlotZoomer(plot->canvas());

        weightWithConfidence = ANALYSE.getWeightByFlat(l_data,nb_valeur,indexArray,statArray);

        if (  anal.isGoodFlat(weightWithConfidence, dataInterval,(max-minInt)) ) {
            output = "Algorithme plat:\nMasse : " + QString::number(weightWithConfidence[0]) + " kg\nIndice de confiance : " + QString::number(weightWithConfidence[1]);
            ui->label_3->setText(output);
        }
        else{
            histoDialog *diag = new histoDialog();
            double maxData[4];

            diag->calculMax(dataInterval,(max-minInt),maxData);
            QVector<double> caractSelected(2);
            caractSelected = diag->caractLoiNormal(maxData,dataInterval,(max-minInt));

            if ( caractSelected[0] > 0 ) {
                output = "Algorithme histogramme:\nMasse : " + QString::number(caractSelected[0]) + " kg\nIndice de confiance : " + QString::number(caractSelected[1]);
                ui->label_3->setText(output);
            }
            else {
                output = "Courbe non traitée\n";
                ui->label_3->setText(output);
            }
        }


        for(int j=0;j<4;j++)
        {
            curve[j]->setRawSamples(xs,dataInterval[j],(max-minInt));
            if ( j < 3 ) {curve[j]->attach(plot);}
            curve[j]->setPen(*color[j]);
        }
        zoomer->setZoomBase();

        plot->updateAxes();
        plot->replot();
        plot->show();
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    tracer(arg1,0,-1);
    if(existFlat) {
        for(int i=0;i<4;i++) {
            for(int j=0;j<sizeFlat[i];j++) {
                curveFlat[i][j]->detach();
            }
        }
    }
    existFlat=false;
}

void MainWindow::affiche(int i,bool select)
{
    if (select == true ) {
        curve[i]->attach(plot);
        selection[i] = 1;
    }
    else {
        curve[i]->detach();
        selection[i] = 0;
    }

    plot->updateAxes();
    plot->replot();
}

void MainWindow::on_balance1_clicked(bool checked)
{
    affiche(0,checked);
    if(!checked && existFlat) {
        for(int j=0;j<sizeFlat[0];j++) {
            curveFlat[0][j]->detach();
        }        
    }
}

void MainWindow::on_balance2_clicked(bool checked)
{
    affiche(1,checked);
    if(!checked && existFlat) {
        for(int j=0;j<sizeFlat[1];j++) {
            curveFlat[1][j]->detach();
        }
    }
}

void MainWindow::on_balance3_clicked(bool checked)
{
    affiche(2,checked);
    if(!checked && existFlat) {
        for(int j=0;j<sizeFlat[2];j++) {
            curveFlat[2][j]->detach();
        }
    }
}

void MainWindow::on_sommebalance_clicked(bool checked)
{
    affiche(3,checked);
    if(!checked && existFlat) {
        for(int j=0;j<sizeFlat[3];j++) {
            curveFlat[3][j]->detach();
        }
    }
}

void MainWindow::on_actionAfficher_triggered()
{
    histoWindow = new histoDialog();
    histoWindow->initHisto();
    histoWindow->setData(l_data,nb_valeur);
    histoWindow->setAttribute(Qt::WA_DeleteOnClose);
    histoWindow->exec();

}

void MainWindow::on_pushButton_2_clicked()
{
    tracer(ui->spinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value());
}

void MainWindow::writeSettings() {
    QSettings settings("METZGER","IHManchots");
    settings.setValue("MainWindow/Size",size());
    settings.setValue("MainWindow/Properties", saveState());
}

void MainWindow::readSettings() {
    QSettings settings("METZGER","IHManchots");
    resize(settings.value("MainWindow/Size",sizeHint()).toSize());
    restoreState(settings.value("MainWindow/Properties").toByteArray());
}

void MainWindow::on_actionAlgorithme_de_plats_triggered()
{
    analyse ANALYSE;
    Flat *indexArray = new Flat();
    Stat *statArray =  new Stat();
    QVector<double> weightWithConfidence(2);
    QString output = "";


    weightWithConfidence = ANALYSE.getWeightByFlat(l_data,nb_valeur,indexArray,statArray);

    output = "Poids : " + QString::number(weightWithConfidence[0]) + "\nIndice de confiance : " + QString::number(weightWithConfidence[1]);
    ui->label_3->setText(output);

    traceFlat(indexArray);
}

void MainWindow::on_actionA_propos_triggered()
{
    QMessageBox::about(this,"A propos","Logiciel crée dans le cadre d'un stage au CNRS dans le département IPHC/DEPE.");
}

int MainWindow::maxArray(QVector<double> index, QVector<double> meanWeight) {
    int max = 0;
    while (meanWeight[max] == 0 && max < 4 ) {
        max++;
    }
    for(int i=max+1;i<4;i++) {
      if (meanWeight[i] != 0) {
          if( index[i] >  index[max] ) {
              max=i;
          }
      }
    }
    return max;
}

void MainWindow::on_pushButton_clicked()
{
    tracer(ui->spinBox->value(),0,-1);
    if(existFlat) {
        for(int i=0;i<4;i++) {
            for(int j=0;j<sizeFlat[i];j++) {
                curveFlat[i][j]->detach();
            }
        }
    }
}

void MainWindow::on_actionEnregistrer_triggered()
{
    QPixmap qPix = QPixmap::grabWidget(plot);
    if(qPix.isNull()) {
        qDebug() << "Failed to capture the plot for saving";
        return;
    }
    QString types("JPEG file (*.jpeg);;"
                  "Portable Network Graphic file (*.png)");
    QString filter;
    QString jpegExt=".jpeg", pngExt=".png";
    //QString suggestedName=restorePath().replace("flxhst","jpeg");
    QString fn = QFileDialog::getSaveFileName(this,tr("Save Image"),QDir::homePath(),types,&filter);
    if (!fn.isEmpty()) {
        if (fn.contains(jpegExt)) {
            fn.remove(jpegExt);
        }
        else if(fn.contains(pngExt)) {
            fn.remove(pngExt);
        }
        if (filter.contains(jpegExt)) {
            fn+=jpegExt;
            qPix.save(fn,"JPEG");
        }
        else if(filter.contains(pngExt)) {
            fn+=pngExt;
            qPix.save(fn,"PNG");
        }
    }
}

void MainWindow::on_actionLisser_la_courbe_triggered()
{
    tracer(ui->spinBox->value(),0,-1,1);
}

void MainWindow::traceFlat(Flat *indexArray) {
    double **xs[4];
    double **ys[4];

    for(int i=0; i<4; i++) {
        sizeFlat[i] = indexArray->indexStart[i].count();

        xs[i] = (double **)calloc(sizeFlat[i],sizeof(double *));
        ys[i] = (double **)calloc(sizeFlat[i], sizeof(double *));
        curveFlat[i] = new QwtPlotCurve *[sizeFlat[i]];
        for(int j=0; j<sizeFlat[i]; j++) {
            int size = indexArray->indexStop[i][j] - indexArray->indexStart[i][j]+1;
            xs[i][j] = (double *)calloc(size,sizeof(double));
            ys[i][j] = (double *)calloc(size,sizeof(double));
            for(int k=indexArray->indexStart[i][j]; k<=indexArray->indexStop[i][j]; k++) {
                xs[i][j][k-indexArray->indexStart[i][j]] = k;
                ys[i][j][k-indexArray->indexStart[i][j]] = l_data[i][k];
            }

            curveFlat[i][j] = new QwtPlotCurve();
            curveFlat[i][j]->setPen(QPen(Qt::magenta));

            curveFlat[i][j]->setRawSamples(xs[i][j], ys[i][j],size);
            if (ui->balance1->isChecked() && (i==0)) {
               curveFlat[i][j]->attach(plot);
            }
            if (ui->balance2->isChecked() && (i==1)) {
               curveFlat[i][j]->attach(plot);
            }
            if (ui->balance3->isChecked() && (i==2)) {
               curveFlat[i][j]->attach(plot);
            }
            if (ui->sommebalance->isChecked() && (i==3)) {
               curveFlat[i][j]->attach(plot);
            }
        }
    }
    existFlat = true;
    plot->updateAxes();
    plot->replot();
    plot->show();
}

void MainWindow::on_actionOuvrir_manchot_triggered()
{
   ui->actionR_sultat->setEnabled(true);
   omWindow->exec();
   filename = omWindow->getFileName();
   if (omWindow->getAuthorizationDraw()) {
       ui->spinBox->setValue(0);
       tracer(0,0,-1);
       this->setWindowTitle(filename);
   }
}

void MainWindow::on_checkBox_2_clicked()
{
    // Affichage Cas simples
    tracer(0,0,-1);
    ui->spinBox->setValue(0);
}

void MainWindow::on_checkBox_clicked()
{
    // Affichage Cas complexes
    ui->spinBox->setValue(0);
    tracer(0,0,-1);
}

void MainWindow::on_actionR_sultat_triggered()
{
    resultWindow = new Resultat(filename);
    resultWindow->exec();
}

void MainWindow::on_actionOptions_triggered()
{
    paramWindow = new Parametres();
    paramWindow->exec();
}
