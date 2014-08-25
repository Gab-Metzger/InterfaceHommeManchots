#include "histodialog.h"
#include "ui_histodialog.h"

histoDialog::histoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::histoDialog)
{
    ui->setupUi(this);
}

histoDialog::~histoDialog()
{
    delete ui;
}

void histoDialog::setData(double**data, int nb_valeur) {
    //Let's write the curves
    double max[4];
    anal.calculMax(data,nb_valeur,max);

    for (int i = 0 ; i < 4; i++ ) {
        double histoMax = setDataInHisto(data,nb_valeur,max[i],i);
        setQuartileInHisto(data,nb_valeur,max[i],histoMax,i);

        histoPlot[i]->replot();
        histoPlot[i]->show();
    }
}


void histoDialog::initHisto(){

    histoPlot[0]  = new QwtPlot(this);
    histoPlot[0]->setTitle("Histogramme 1");
    histo[0] = new QwtPlotHistogram("Histo");
    ui->histo1Layout->addWidget(histoPlot[0]);

    histoPlot[1]  = new QwtPlot(this);
    histoPlot[1]->setTitle("Histogramme 2");
    histo[1] = new QwtPlotHistogram("Histo");
    ui->histo2Layout->addWidget(histoPlot[1]);

    histoPlot[2]  = new QwtPlot(this);
    histoPlot[2]->setTitle("Histogramme 3");
    histo[2] = new QwtPlotHistogram("Histo");
    ui->histo3Layout->addWidget(histoPlot[2]);

    histoPlot[3]  = new QwtPlot(this);
    histoPlot[3]->setTitle("Histogramme Somme");
    histo[3] = new QwtPlotHistogram("Histo");
    ui->histo4Layout->addWidget(histoPlot[3]);

    Histogramme(histo[0],histoPlot[0]);
    Histogramme(histo[1],histoPlot[1]);
    Histogramme(histo[2],histoPlot[2]);
    Histogramme(histo[3],histoPlot[3]);

    //Create mean,median mark
    for(int i=0;i<5;i++) {
        for(int j=0;j<4;j++) {
            curve_Q[i][j] = new QwtPlotMarker();
            if ( i == 0 ) {
                curvenormal[j] = new QwtPlotCurve();
            }
        }
    }
}

void histoDialog::Histogramme(QwtPlotHistogram *histo, QwtPlot *plot_H) {
    histo->setPen( QPen( Qt::black ) );
    histo->setYAxis( QwtPlot::yLeft );
    histo->setXAxis( QwtPlot::xBottom );
    histo->attach(plot_H);

    plot_H->show();
}

double histoDialog::setDataInHisto(double**data,int nb_valeur,double max,int c)
{
        double histoMax=0;
        QVector<QwtIntervalSample> tmp_sample(nb_valeur);
        QVector<double> element_interval;
        int size = floor(10 * max);
        anal.initInter(&element_interval,data[c],max,nb_valeur,&histoMax);

        for(int i = 0 ;i< size ;i++) {
            tmp_sample.append(QwtIntervalSample(element_interval[i], double(i/10.0) , double(i/10.0) + 0.1 ));
        }
        histo[c]->setData(new QwtIntervalSeriesData(tmp_sample));
        element_interval.clear();

        return histoMax;
}

void histoDialog::setQuartileInHisto(double**data,int nb_valeur,double max,double maxHisto,int c) {

    int nb_element_R=0,i=0;
    double moy=0.0,var=0.0,*quartile=(double*)calloc(3,sizeof(double)),d=0.0,f=0.0, valX =0.0;
    char titre[200];

    for(int j = 0 ;j<5;j++) {
        curve_Q[j][c]->detach();
    }

    st.moustach(data[c],nb_valeur,&d,&f,&moy,&var,&nb_element_R,quartile);

    if ( c < 3 ) {
        sprintf(titre,"Histogramme %d\n(Moy. : %f ,Var. : %f)\n",c+1,moy,var);
    }
    else {
        sprintf(titre,"Histogramme somme\n(Moy. : %f ,Var. : %f)\n",moy,var);
    }

    QwtText title(titre);
    title.setFont(QFont("Helvetica", 8));
    histoPlot[c]->setTitle(title);

    //debut normal
    anal.initLoiNormal(&loiNormal[c],moy,maxHisto,max);

     xs[c] = (double*)calloc((10*max)+1,sizeof(double));
     int size = floor(10*max)+1;
     for(int j=0;j<size;j++) {
          xs[c][j]=j/10.0;
     }

    curvenormal[c]->setRawSamples(xs[c],loiNormal[c],size);
    curvenormal[c]->attach(histoPlot[c]);
    // fin normal

    for(i=0;i<5;i++)
    {

        if ( i < 3 ) {
            valX=quartile[i];
            curve_Q[i][c]->setLinePen(Qt::red);
        }
        else if ( i == 3 ) {
            valX = d;
            curve_Q[3][c]->setLinePen(Qt::green);
        }
        else {
            valX = f;
            curve_Q[4][c]->setLinePen(Qt::green);
        }

        curve_Q[i][c]->setXValue(valX);
        curve_Q[i][c]->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
        curve_Q[i][c]->setLineStyle(QwtPlotMarker::VLine);

        curve_Q[i][c]->attach(histoPlot[c]);
    }
}
