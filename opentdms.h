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
    int Manchot_txt(QList<QString> infoDb,QString cheminDossierTdms, QString cheminDossierArrive, int idManchot);

    Passage passage;
    tdms TDMS;
    Detections dt;

private:
    void FichierTdmsErreurDossier(QString chemin_fichier, QString nomfichier, QString cause);
    /**
     * @brief fichier_pourcentage
     * @param trajet_dest
     * @param nb_cas
     */
    void fichier_pourcentage(QString trajet_dest, double nb_cas[2]);

    void analyse3plateau( QFileInfoList listFichierTdms,char* trajet_dest,QFileInfo InfoDossier ,QString trajet_TDMS,int i,double *nb_cas);

    void ParcourFichierJour(QString trajet_prov, QDir Dossier_txt, QFileInfo InfoDossier, double nb_cas[2]);

    void FichierTdmsErreurDossier(QFileInfo InfoDossier,QString heure,int num_plateau,QString trajet_dest,QString cause);
};

#endif // OPENTDMS_H
