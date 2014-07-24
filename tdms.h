#ifndef TDMS_H
#define TDMS_H

#include <nilibddc.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#define ddcChk(f) if (ddcError = (f), ddcError < 0) goto Error;
#ifdef nullChk
#undef nullChk
#endif
#define nullChk(p) if (!(p)) { ddcError = DDC_OutOfMemory; goto Error; }

class tdms
{
public:
    tdms();

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
    /**
     * @brief fichiertdms_erreur
     * @param chemin_fichier
     * @param nomfichier
     * @param cause
     */
};

#endif // TDMS_H
