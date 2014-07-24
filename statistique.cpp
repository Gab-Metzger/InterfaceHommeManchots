#include "statistique.h"

statistique::statistique()
{
}

QList<double> statistique::copy_seuil_min(double* data1,  QList<double> data2, int nb_value, double seuil_min) {
    int j, k = 0;
    for (j = 0; j < nb_value ; j++) {
        if (data1[j] > seuil_min) {
            data2.insert(k,data1[j]);
            k++;
        }
    }
    return data2;
}

bool statistique::compare(double &a, double &b) {
    return a < b ? false : true;
}

void statistique::getStatData(double *data, int numDataValues, double* moy, double* var) {
        int i;
        double sum[2]={0,0};

        for (i = 0; i < numDataValues; i++) {
            sum[0] += data[i];
            sum[1] += (data[i] * data[i]);
        }

        *moy = sum[0] / numDataValues;
        sum[1] = sum[1] / numDataValues;
        *var = sum[1] - (*moy) * (*moy);
}

void statistique::moustach(double* data, int nb_element, double *d, double *f, double *moy, double*var, int* nb_element_R, double quartile[3]) {

    QList<double> data_t;

    data_t = copy_seuil_min(data, data_t, nb_element, 0.5);
    int nb_element_t = data_t.size();
    quartile[0] = -1.0;
    quartile[1] = -1.0;
    quartile[2] = -1.0;

    *moy = 0.0;
    *var = 0.0;
    *f = 0.0;
    *d = 0.5;
    *nb_element_R = 0;

    if (nb_element_t > 0) {
        qSort(data_t);

        quartile[0] = data_t.at(nb_element_t / 4);
        if ((nb_element_t % 2) == 1) { quartile[1] = data_t.at(nb_element_t / 2); }
        else { quartile[1] = (data_t.at(nb_element_t / 2) + data_t.at((nb_element_t / 2) - 1) ) / 2; }
        quartile[2] = data_t.at((3 * nb_element_t) / 4);
        double diff = 1.5*(quartile[2] - quartile[0]);

        double borneInf = (quartile[0] - diff);
        double borneSup = (quartile[2] + diff);
        int i = 0, d_i = 0, f_i = 0;
        *var = 0.0;

        while ( (i < nb_element_t ) && data_t[i] <= borneSup ) {
            if (*d <= borneInf ) {
                *d = data_t[i];
                d_i = i;
            }
            else {
                *moy += data_t[i];
                *var += data_t[i] * data_t[i];
            }
            i++;
        }
        *f = data_t[i-1];

        f_i = i;
        d_i++;

        *nb_element_R = (f_i - d_i);

        if ( *nb_element_R > 0 ) {
            *moy = *moy / *nb_element_R;

            *var = *var / *nb_element_R;
            *var = *var - (*moy * *moy);
        }
    }
}

double* statistique::getNewData(double *data, int nb_element, double d, double f, int nb_element_R){
    int 	i, j = 0;
    double *data_m = (double*)malloc(nb_element_R*sizeof(double));

    for (i = 0; i < nb_element; i++) {
        if (data[i] >= d && data[i] <= f) {
            data_m[j] = data[i];
            j++;
        }
    }

    return data_m;
}
