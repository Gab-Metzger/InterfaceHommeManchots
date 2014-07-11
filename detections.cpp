#include "detections.h"
#include "ui_detections.h"

Detections::Detections(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Detections)
{
    ui->setupUi(this);
    plainModel = new QSqlQueryModel();
    //initializeModel(plainModel);
    //createView(plainModel);
}

Detections::~Detections()
{
    delete plainModel;
    delete ui;
}

void Detections::initializeModel(QSqlQueryModel *model)
{
    model->setQuery("SELECT antenne_id, animaux_id, date_arrivee, date_depart FROM detections WHERE animaux_id=4 LIMIT 0,100;");
    model->setHeaderData(1,Qt::Horizontal, QObject::tr("Antenne"));
    model->setHeaderData(2,Qt::Horizontal, QObject::tr("Id Manchot"));
    model->setHeaderData(3,Qt::Horizontal, QObject::tr("Date arrivée"));
    model->setHeaderData(4,Qt::Horizontal, QObject::tr("Date départ"));
}

QTableView* Detections::createView(QSqlQueryModel *model)
{
    ui->tableView->setModel(model);

    ui->tableView->show();

    return ui->tableView;
}

int Detections::spaceBetweenHour(int ourHour, int endTheo,int line) {

    QSqlQuery query;
    query.exec("SELECT SUBSTR(date_arrivee,12), date_arrivee_ms, SUBSTR(date_depart,12), date_depart_ms, animaux_id FROM detections WHERE animaux_id=441;");
    query.seek(line);
    int res=0;
    int id_manchot[2]={0,0};

    while( id_manchot[1] == 0 &&  query.next() ) {

        int hour[2][4];
        QString inHour = query.value(0).toString();
        hour[0][3] = query.value(1).toInt();
        QString outHour = query.value(2).toString();
        hour[1][3] = query.value(3).toInt();
        sscanf(qPrintable(inHour),"%d:%d:%d", &hour[0][0],&hour[0][1],&hour[0][2]);
        sscanf(qPrintable(outHour),"%d:%d:%d", &hour[1][0],&hour[1][1],&hour[1][2]);

        int begin=0,end=0;
        for(int i=0;i<3;i++) {
            begin = 60*begin + hour[0][i];
            end = 60*end + hour[1][i];
        }
        begin = (1000*begin + hour[0][3]);
        end = (1000*end + hour[1][3]);

        if ( (ourHour > begin-1000) && (ourHour < end) && (id_manchot[0]==0)) {
            id_manchot[0] = query.value(4).toInt();
            qDebug() <<"1"<< inHour << id_manchot[0];
        }

        if ((endTheo > begin) && (endTheo < end) && id_manchot[0] != 0 ) {
            id_manchot[1] = query.value(4).toInt();
            qDebug() << "2" << inHour << id_manchot[1];
        }
        res++;
    }
    return res-1;
}

QVector<QVector<int> > Detections::extractRaws(int plate, QString date, QString hour) {
    QVector<QVector<int> > res;
    int p1 = 0, p2 = 0, hourMsDepart, hourMsArrivee,j=0;
    QString dateQuery;

    res.resize(3);
    //Extract the two plate's values
    p1 = plate/10;
    p2 = plate % 10;
    //Extract the good date-hour format
    dateQuery = date + " " + hour;
    //Query
    QSqlQuery query;
    query.prepare("SELECT animaux_id, date_arrivee, date_arrivee_ms, date_depart, date_depart_ms FROM detections WHERE antenne_id IN  (:p1,:p2) AND date_arrivee LIKE ':date%'");
    query.bindValue(":p1",p1);
    query.bindValue(":p2",p2);
    query.bindValue(":date",dateQuery);
    query.exec();
    //Resize tab with query rows
    for(int i=0; i<3; i++) {
        res[i].resize(query.size());
    }
    //Get and save the query results
    while(query.next()) {
        int hour[2][4];
        QString inHour = query.value(1).toString();
        hour[0][3] = query.value(2).toInt();
        QString outHour = query.value(3).toString();
        hour[1][3] = query.value(4).toInt();
        sscanf(qPrintable(inHour),"%d:%d:%d", &hour[0][0],&hour[0][1],&hour[0][2]);
        sscanf(qPrintable(outHour),"%d:%d:%d", &hour[1][0],&hour[1][1],&hour[1][2]);

        hourMsDepart=0;
        hourMsArrivee=0;
        for(int i=0;i<3;i++) {
            hourMsDepart = 60*hourMsDepart + hour[0][i];
            hourMsArrivee = 60*hourMsArrivee + hour[1][i];
        }
        res[0][j] = query.value(0).toInt();
        res[1][j] = hourMsDepart;
        res[2][j] = hourMsArrivee;
        j++;
    }
    return res;
}
