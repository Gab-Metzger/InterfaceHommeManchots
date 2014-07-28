#include "lecture_fichiers.h"

lecture_fichiers::lecture_fichiers()
{
}

double** lecture_fichiers::allocation_m(QString titre, int* nb_passage,int *num_passage,char *cas,QString *dateTime,bool caseTab[])
{
    FILE *g;
    char lecture_v[300];
    int taille=0,k=0;
    char date[12],heure[13];


    errno_t err = fopen_s(&g, titre.toStdString().c_str(), "r+");
    if (err)
        printf_s("The file fscanf.out was not opened \n");

    double** data= (double**)calloc(4,sizeof(double*));

    k=0;
    int validate=-1;

    while ( fgets(lecture_v,300,g) && ( validate != (*num_passage + 1) ) ) {
        sscanf(lecture_v,"%s %s %d %c\n",date,heure,&taille,cas);

        *dateTime=QString("").append(date).append(" ").append(heure);


        if ( k == *num_passage && caseTab[0] && caseTab[1] ){
            validate = *num_passage;
        }
        else if ( caseTab[0] && !caseTab[1] && *cas == 's' ) {
            validate += 1;
        }
        else if ( caseTab[1] && !caseTab[0] && *cas == 'c' ) {
            validate += 1;
        }

        if ( validate == *num_passage ) {
            data[0] = (double*)calloc(taille,sizeof(double));
            data[1] = (double*)calloc(taille,sizeof(double));
            data[2] = (double*)calloc(taille,sizeof(double));
            data[3] = (double*)calloc(taille,sizeof(double));

            *nb_passage = -1;
            validate += 1;
        }
        else {
          k++;
        }
    }
    qDebug() << validate;
     qDebug() << *nb_passage;
    if ( validate != (*num_passage + 1) ) {
        validate = -1;
        *num_passage = -1;
    }
    else {
       *num_passage = k;
    }
    *nb_passage = validate + 1;


    fclose(g);
    return data;
}

int lecture_fichiers::lire_fichier(QString titre, double*** data, int* nb_valeur, int* nb_passage, char* cas, int num_passage,QString *dateTime, bool caseTab[2])
{
    FILE *f;
    int k = 0, sortie = 0,incr=0,num_R_passage=num_passage;
    char* lecture = (char*)malloc(3 * (sizeof(double)+sizeof(char)));
    QString chemin_ref="";
    int res = 0;
    int n = titre.length();

    chemin_ref.append(titre);
    chemin_ref.replace(n-14,7,"reference");

    (*data)=allocation_m(chemin_ref,nb_passage,&num_R_passage,cas,dateTime,caseTab);

    errno_t err = fopen_s(&f,titre.toStdString().c_str(), "r+");

    if (err) {
        printf_s("The file fscanf.out was not opened\n");
        res = 1;
    }

    k = 0;
    incr = 0;

    while ( fgets(lecture, 100, f) && !sortie && (num_R_passage != -1) ) {

        if ( strcmp(lecture, "fin\n") == 0 ) {
            incr++;
        }
        else if ( incr == num_R_passage ) {
            sscanf(lecture,"%lf %lf %lf\n",&(*data)[0][k],&(*data)[1][k],&(*data)[2][k]);
            (*data)[3][k] = (*data)[0][k] + (*data)[1][k] + (*data)[2][k];

            k++;
        }
        else if (incr > num_R_passage ){
            sortie = 1;
        }
    }
    *nb_valeur = k;
    chemin_ref.clear();
    free(lecture);
    if ( num_passage > num_R_passage ) {
        res = 2;
    }

    fclose(f);
    return res;
}
