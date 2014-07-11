#include "histodialog.h"
#include "ui_histodialog.h"
#include "statistique.h"

histoDialog::histoDialog(double**data, int nb_valeur, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::histoDialog)
{
    ui->setupUi(this);

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
        }
    }

    //Let's write the curves
    for (int i = 0 ; i < 4; i++ ) {
        setDataInHisto(data,nb_valeur,i);
        setQuartileInHisto(data,nb_valeur,i);
        histoPlot[i]->replot();
        histoPlot[i]->show();
    }
}

histoDialog::~histoDialog()
{
    delete ui;
}


void histoDialog::Histogramme(QwtPlotHistogram *histo, QwtPlot *plot_H) {
    histo->setPen( QPen( Qt::black ) );
    histo->setYAxis( QwtPlot::yLeft );
    histo->setXAxis( QwtPlot::xBottom );
    histo->attach(plot_H);

    plot_H->show();
}

void histoDialog::calculMax(double **data, int nb_valeur, double max[4]) {
    max[0]=data[0][0] ;
    max[1]=data[1][0] ;
    max[2]=data[2][0] ;
    max[3]=max[0];

    for(int i=0; i<4; i++) {
        for (int x = 0; x<nb_valeur; x++) {
            if (data[i][x] > max[i]) {
                max[i] = data[i][x];
            }
        }
    }
}

void histoDialog::setDataInHisto(double**data,int nb_valeur,int c)
{
        double max[4];
        calculMax(data,nb_valeur,max);
        QVector<QwtIntervalSample> tmp_sample(nb_valeur);
        QVector<double> element_interval(10*max[c] + 1 );
        int i=0,E=0;

        element_interval.fill(0.0);

        for(i = 0 ; i < nb_valeur; i++ ) {
            if ( data[c][i] > 0.5 ) {

                E = floor( 10 * data[c][i]);
                element_interval.replace(E,element_interval.at(E) + 1.0);
            }
        }

        for(i = 0 ;i<(10 * max[c]);i++) {
            tmp_sample.append(QwtIntervalSample(element_interval.at(i), double(i/10.0) , double(i/10.0) + 0.1 ));
        }

        histo[c]->setData(new QwtIntervalSeriesData(tmp_sample));
        element_interval.clear();
}

void histoDialog::setQuartileInHisto(double**data,int nb_valeur,int c) {

    int nb_element_R=0,i=0;
    double moy=0.0,var=0.0,*quartile=(double*)calloc(3,sizeof(double)),d=0.0,f=0.0;
    char titre[200];

    moy_p=0.0;
    var_p=0.0;
    var_s=0.0;

    for(int j = 0 ;j<5;j++) {
        curve_Q[j][c]->detach();
    }
    statistique STAT;
    STAT.moustach(data[c],nb_valeur,&d,&f,&moy,&var,&nb_element_R,quartile);

    double valX =0.0;

    if ( c < 3 ) {
        sprintf(titre,"Histogramme %d\n(Moy. : %f ,Var. : %f)\n",c+1,moy,var);
        moy_p += (1 / var) * moy;
        var_p += (1/var);
        var_s += var;
    }
    else {
        sprintf(titre,"Histogramme somme\n(Moy. : %f ,Var. : %f)\n",moy,var);

        moy_p = moy_p / var_p;
        var_s = var_s /3;

        //moyenne->setText("moyenne: "+QString::number(moy_p));
        //variance->setText("variance: "+QString::number(var_s));

        moy_p=0.0;
        var_p=0.0;
        var_s=0.0;
    }

    QwtText title(titre);
    title.setFont(QFont("Helvetica", 8));
    histoPlot[c]->setTitle(title);

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
