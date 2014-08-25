#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QList>
#include <QDebug>

class Statistique
{
public:
    Statistique();
    /**
     * @brief moustach
     * @param data
     * @param nb_element
     * @param d
     * @param f
     * @param moy
     * @param var
     * @param nb_element_R
     * @param quartile
     */
    void moustach(double* data, int nb_element, double *d, double *f, double *moy, double*var, int* nb_element_R, double quartile[3]);
    void getStatData(double *data, int numDataValues, double* moy, double* var);

private:


    /**
     * @brief getNewData
     * @param data
     * @param nb_element
     * @param d
     * @param f
     * @param nb_element_R
     * @return
     */
    double* getNewData(double *data, int nb_element, double d, double f, int nb_element_R);
    /**
     * @brief compare
     * @param a
     * @param b
     * @return
     */
    bool compare(double &a, double &b);
    /**
     * @brief copy_seuil_min
     * @param data1
     * @param data2
     * @param nb_value
     * @param seuil_min
     * @return
     */
    QList<double> copy_seuil_min(double* data1, QList<double> data2, int nb_value, double seuil_min);
};

#endif // STATISTIQUE_H
