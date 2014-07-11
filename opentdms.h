#ifndef OPENTDMS_H
#define OPENTDMS_H


#include "opentdms_global.h"
#include <nilibddc.h>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QDebug>
#include <math.h>
#include <QtWidgets/QMessageBox>

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#define ddcChk(f) if (ddcError = (f), ddcError < 0) goto Error;
#ifdef nullChk
#undef nullChk
#endif
#define nullChk(p) if (!(p)) { ddcError = DDC_OutOfMemory; goto Error; }



class OPENTDMSSHARED_EXPORT OpenTdms
{

public:
    OpenTdms();
    /**
     * @brief creation_txt
     * @param trajet_prov
     * @param trajet_dest
     */
    int creation_txt(QString trajet_prov, QString chemin_Dossier_arrive, QString date,QString heure, QString plateau);
    /**
     * @brief lecture_tdms
     * @param trajet
     * @param data
     * @param nb
     * @param num_plateau
     * @return
     */
    int lecture_tdms(char* trajet , double** data, int nb[3], int num_plateau);
    /**
     * @brief freeTableauDim2
     * @param tableau
     * @param x
     */
    void freeTableauDim2(double** tableau, int x);
private:
    /**
     * @brief ReadFile
     * @param nb_valeur
     * @param fichier
     * @param data
     * @return
     */
    int ReadFile(int* nb_valeur, char * fichier, double** data);
    /**
     * @brief ReadGroups
     * @param file
     * @param nb_valeur_t
     * @param data
     * @return
     */
    int ReadGroups(DDCFileHandle file, int* nb_valeur_t, double** data);
    /**
     * @brief ReadChannels
     * @param group
     * @param nb_valeur_t
     * @param data_l
     * @return
     */
    int ReadChannels(DDCChannelGroupHandle group, int* nb_valeur_t, double** data_l); 

    void fichiertdms_erreur(QString chemin_fichier, QString nomfichier, QString cause);
    /**
     * @brief fichier_pourcentage
     * @param trajet_dest
     * @param nb_cas
     */
    void fichier_pourcentage(QString trajet_dest, double nb_cas[2]);
};

#endif // OPENTDMS_H
