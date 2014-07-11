#ifndef PASSAGE_H
#define PASSAGE_H

#include <iostream>
#include <QDebug>
#include <math.h>
#include <QtConcurrent/QtConcurrentRun>

#define BARRE 0.15
#define BRUIT 0.08
#define BARRE_M 1.0
#define NOMBRE_M 150
#define TEMPS_VOL 50
#define NB_REP 10
#define LGMINPIC 10
#define TAILLEMINPIC 2.5

typedef struct infoCourbe Analyse;

class Passage
{
public:
    Passage();
    /**
     * @brief detection_passage
     * @param data
     * @param nb
     * @param titre_valeur
     * @param titre_reference
     * @param heure
     * @return
     */
    int detection_passage(double** data, int nb, QString titre_valeur, QString titre_reference, int heure, double nb_cas[]);

private:
    /**
     * @brief InitAnalyse
     * @param InfoAnalyse
     * @param nb
     */
    void InitAnalyse(Analyse *InfoAnalyse, int nb);
    /**
     * @brief detection_cas
     * @param front_M
     * @return
     */
    int detection_cas(int front_M[3]);
    /**
     * @brief detection_front
     * @param front_M
     * @param front_D
     * @param i
     * @param data
     * @param j
     */
    void detection_front(double** data, int i, Analyse *InfoAnalyse);
    /**
     * @brief ecriture_fichier
     * @param f
     * @param data
     * @param k
     * @param InformationAnalyse
     */
    void ecriture_fichier(FILE* f, double** data, Analyse *InfoAnalyse, int indiceFinPicPrec[]);
    /**
     * @brief indice_min
     * @param index_0
     * @param pic
     * @return
     */
    int indice_min(int index_0[3] , int pic[]);
    /**
     * @brief detection_cote
     * @param data
     * @param j
     * @param k
     * @return
     */
    void detection_cote(double** data,int k,Analyse *InfoAnalyse);
    /**
     * @brief detection_debut
     * @param data
     * @param k
     * @param InformationAnalyse
     * @return
     */
    void detection_debut(double** data,int k,Analyse *InfoAnalyse);
    /**
     * @brief detection_debut_ensemble
     * @param data
     * @param InformationAnalyse
     * @return
     */
    void detection_debut_ensemble(double** data,Analyse *InfoAnalyse);
    /**
     * @brief fin_passage
     * @param incr
     * @param front_D
     * @param front_M
     * @param cmpt
     * @return
     */
    int fin_passage(double **data, Analyse *InfoAnalyse);
    /**
     * @brief validation_analyse
     * @param pic
     * @param j
     * @param nb_max
     * @return
     */
    int validation_analyse(Analyse *InfoAnalyse);
    /**
     * @brief detection_bruit
     * @param data
     * @param k
     * @param InformationAnalyse
     * @return
     */
    int detection_bruit(double** data,int k,Analyse *InfoAnalyse);
    /**
     * @brief front_bas
     * @param data
     * @param courbe
     * @param InfoAnalyse
     * @return
     */
    int front_bas(double** data, int courbe, int refIndice, Analyse *InfoAnalyse);
    /**
     * @brief affichageInfoAnalyse
     * @param InfoAnalyse
     */
    void affichageInfoAnalyse(Analyse *InfoAnalyse);

    int intersectionCourbe(Analyse *InfoAnalyse,int a ,int b);

    void maxFinPossible(Analyse *InfoAnalyse);

    void FinPic(double** data,int k,Analyse *InfoAnalyse);
};

#endif // PASSAGE_H
