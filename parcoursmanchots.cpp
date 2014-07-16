#include "parcoursmanchots.h"

ParcoursManchots::ParcoursManchots()
{
}

QVector<QString> ParcoursManchots::searchRightFile(int idManchot) {
    QString fileName,fileNameRef;
    QVector<QString> fileNames(2);

    //Search wich file open
    fileName = QDir::homePath() + "/.ihmanchots/manchot " + idManchot + ".txt";
    fileNameRef = QDir::homePath() + "/.ihmanchots/manchotRef " + idManchot + ".txt";

    //Insert files location into QVector
    fileNames[0] = fileName;
    fileNames[1] = fileNameRef;

    return fileNames;
}

QVector<QVector<double> > ParcoursManchots::processLineAllocation(QString line) {
    QTextStream decomposeLine(&line);
    QString dateHour, difficulty;
    int size;
    QVector<QVector<double> >data(4);

    decomposeLine >> dateHour >> size >> difficulty;
    for(int i=0; i<4; i++) data[i].resize(size);

    return data;
}

QVector<QVector<double> > ParcoursManchots::processLineSaveData(QVector<QVector<double> >data, QString line) {
    QTextStream decomposeLine(&line);
    double one, two, three;

    decomposeLine >> one >> two >> three;
    data[0].append(one);
    data[1].append(two);
    data[2].append(three);
    data[3].append(one+two+three);

    return data;
}

QVector<QVector<double> > ParcoursManchots::openAndDigIntoFiles(QVector<QString> fileNames) {
    QVector<QVector<double> >data;

    //Opening files
    try {
        QFile file(fileNames[0]);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw QString("Error while opening" + fileNames[0]);
        }
        QFile fileRef(fileNames[1]);
        if (!fileRef.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw QString("Error while opening" + fileNames[1]);
        }

        QTextStream inRef(&fileRef);
        QTextStream in(&file);
        while(!inRef.atEnd()) {
            QString lineRef = inRef.readLine();
            data = processLineAllocation(lineRef);
            while((!in.readLine().compare("fin\n")) || (!in.atEnd())) {
                QString line = in.readLine();
                processLineSaveData(data,line);
            }
        }
    }
    catch(const QString string) {
        qDebug() << string;
    }

    return data;
}
