#ifndef LECTURE_FICHIERS_H
#define LECTURE_FICHIERS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>
#include <QDebug>

class lecture_fichiers
{
public:
    lecture_fichiers();
    /**
     * @brief lire_fichier
     * @param titre
     * @param data
     * @param nb_valeur
     * @param nb_passage
     * @param cas
     * @param num_passage
     * @param heure
     */
    int lire_fichier(QString titre, double ***data, int *nb_valeur, int *nb_passage, char *cas, int num_passage, QString *dateTime, bool caseTab[]);
private:
    /**
     * @brief allocation_m
     * @param titre
     * @param nb_passage
     * @param num_passage
     * @param cas
     * @param heure
     * @return
     */
    double **allocation_m(QString titre, int* nb_passage, int *num_passage, char *cas, QString *dateTime, bool caseTab[]);
};

#endif // LECTURE_FICHIERS_H
