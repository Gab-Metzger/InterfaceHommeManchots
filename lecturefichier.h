#ifndef LECTUREFICHIER_H
#define LECTUREFICHIER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>
#include <QDebug>

class LectureFichier
{
public:
    LectureFichier();
    /**
     * @brief allocation_m
     * @param titre
     * @param nb_passage
     * @param num_passage
     * @param cas
     * @param heure
     * @return
     */
    double **allocation_m(QString titre, int* nb_passage, int *num_passage, char *cas, int *numTransition, QString *dateTime, bool caseTab[]);
};

#endif // LECTUREFICHIER_H
