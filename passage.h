#ifndef PASSAGE_H
#define PASSAGE_H

#include <iostream>
#include <QDebug>
#include <math.h>
#include <QtConcurrent/QtConcurrentRun>
#include "bruit.h"
#include "detections.h"
#include <QFile>

#define NOMBRE_M 150
#define TEMPS_VOL 50
#define NB_REP 10
#define LGMINPIC 10
#define NOMBRESTAG 2000
#define ECARTHAUT 0.1

typedef struct infoCourbe{

    int NombreData;
    double Data[3];

    int indiceCourant[3];
    int indiceValeur0[3];
    int indiceFinPassage;

    int informationPic[3];
    int courbePremierPic;

    int nb_front_M[3];
    int front_D[3];

    int DroitEcriture;

}Analyse;

typedef struct Manchot {
    QVector<QVector<int> > tableau;
    int IndiceCourant;

    QString date;
    int heure;
    int plateau;
}Manchot;

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
    int detection_passage(double** data, int nb, QString titre_valeur,QString date,int heure,int plateau, double nb_cas[],int idManchot);
    bruit Bruit;
    Detections Dt;

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
    void detection_front(double** data, int i, Analyse *InfoAnalyse, Pic RefPic);
    /**
     * @brief ecriture_fichier
     * @param f
     * @param data
     * @param k
     * @param InformationAnalyse
     */
    void ecriture_fichier(QString titre_valeur,Manchot *Adelie, double** data,Analyse *InfoAnalyse,int indiceFinPicPrec[3],Pic RefPic[3]);
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
    void detection_cote(double** data, int k, Analyse *InfoAnalyse, Pic *RefCote, Pic RefPic);
    /**
     * @brief detection_debut
     * @param data
     * @param k
     * @param InformationAnalyse
     * @return
     */
    void detection_debut(double** data, int k, Analyse *InfoAnalyse, Pic *RefPic, Pic *RefCote);
    /**
     * @brief detection_debut_ensemble
     * @param data
     * @param InformationAnalyse
     * @return
     */
    void detection_debut_ensemble(double** data, Analyse *InfoAnalyse, Pic RefPic[]);
    /**
     * @brief fin_passage
     * @param incr
     * @param front_D
     * @param front_M
     * @param cmpt
     * @return
     */
    int fin_passage(double **data, Analyse *InfoAnalyse, Pic RefPic[]);
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
    int detection_bruit(double** data, int k, Analyse *InfoAnalyse, Pic RefPic);
    /**
     * @brief front_bas
     * @param data
     * @param courbe
     * @param InfoAnalyse
     * @return
     */
    int front_bas(double** data, int courbe, int *refIndice, Analyse *InfoAnalyse, Pic *RefPic);
    /**
     * @brief affichageInfoAnalyse
     * @param InfoAnalyse
     */
    void affichageInfoAnalyse(Analyse *InfoAnalyse);

    int intersectionCourbe(Analyse *InfoAnalyse, double **data, Pic RefPic[], int a , int b);

    int maxFinPossible(Analyse *InfoAnalyse, double **data, Pic RefPic[]);

    void FinPic(double** data, int k, Analyse *InfoAnalyse, Pic *RefPic);

    void testCourbeComm(int a,int b,int l,double stat[3][3],double** data,Analyse *InfoAnalyse);

    int intervalCourbe(Analyse *InfoAnalyse,int r,int l);

    void MiseAJourFront(Analyse *InfoAnalyse,double** data,Pic RefPic[]);


};

#endif // PASSAGE_H
