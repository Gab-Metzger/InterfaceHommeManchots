#ifndef OPENTDMS_H
#define OPENTDMS_H


#include "opentdms_global.h"

#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QDebug>
#include <math.h>
#include <QtWidgets/QMessageBox>

#include "passage.h"
#include "tdms.h"
#include "detections.h"
#include "analyse.h"
#include "lecturefichier.h"

class OPENTDMSSHARED_EXPORT OpenTdms
{

public:
    OpenTdms();


    /**
     * @brief creation_txt
     * @param trajet_prov
     * @param trajet_dest
     */
    int creation_txt(QString chemin_Dossier_tdms, QString chemin_Dossier_arrive,QString infoFichier,int idManchot);
    int ManchotTxt(QString cheminDossierTdms, QString cheminDossierArrive, int idManchot);

    void UpadateAllManchot(QString directoryTdms, QString directoryTxt , QDate start, QDate end);
    void analyseFichier(QFuture<void> thread,QString chemin);
    int lireFichier(QString titre, double*** data, int* nb_valeur, int* nb_passage, char* cas, int *numTransition, int num_passage,QString *dateTime, bool caseTab[2]);

private:
    Analyse anal;
    Passage passage;
    tdms TDMS;
    Detections dt;
    LectureFichier lf;

    void FichierTdmsErreurDossier(QString chemin_fichier, QString nomfichier, QString cause);
    void SearchAllManchot(QString cheminDossierTdms, QString cheminDossierArrive, QDate start, QDate end);
    QStringList dayInterval(QString directory,QDate start, QDate end, QString year);
    QStringList yearInterval(QString directory,QDate start, QDate end);
    /**
     * @brief fichier_pourcentage
     * @param trajet_dest
     * @param nb_cas
     */
    void fichier_pourcentage(QString trajet_dest, double nb_cas[2]);

    void analyse3plateau( QFileInfoList listFichierTdms,char* trajet_dest,QFileInfo InfoDossier ,QString trajet_TDMS,int i,double *nb_cas);

    void FichierTdmsErreurDossier(QFileInfo InfoDossier,QString heure,int num_plateau,QString trajet_dest,QString cause);
    void ParcourFichierJour(QString cheminDossierTdms, QDir Dossier_txt, QString InfoDossier);
    void waitForRead(QFile* fichier,QString cheminTxt);
};

#endif // OPENTDMS_H
