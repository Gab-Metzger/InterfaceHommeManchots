#include "lecture_fichiers.h"

lecture_fichiers::lecture_fichiers()
{
}

double** lecture_fichiers::allocation_m(QString titre, int* nb_passage,int num_passage,char *cas,char heure[13])
{
    FILE *g;
    char lecture_v[300];
    int taille=0,k=0,taille_max=0;

    errno_t err = fopen_s(&g, titre.toStdString().c_str(), "r+");
    if (err)
        printf_s("The file fscanf.out was not opened \n");

    fgets(lecture_v,6,g);
    *nb_passage = atoi(lecture_v);


    double** data= (double**)calloc(4,sizeof(double*));

    if ( *nb_passage < (num_passage-1) ) {
        perror("impossible nb passage trop faible");
    }

    k=0;
    while ( fgets(lecture_v,300,g) ) {
        if ( k == num_passage ) {
            sscanf(lecture_v,"%s %d %c\n",heure,&taille,cas);

            data[0] = (double*)calloc(taille,sizeof(double));
            data[1] = (double*)calloc(taille,sizeof(double));
            data[2] = (double*)calloc(taille,sizeof(double));
            data[3] = (double*)calloc(taille,sizeof(double));
            // à renvoyer
            if ( taille > taille_max ) {taille_max = taille;}
        }

        k++;
    }

    fclose(g);
    return data;
}

int lecture_fichiers::lire_fichier(QString titre, double*** data, int* nb_valeur, int* nb_passage, char* cas, int num_passage, char heure[13])
{
    FILE *f;
    int k = 0, sortie = 0,incr=0;
    char* lecture = (char*)malloc(3 * (sizeof(double)+sizeof(char)));
    QString chemin_ref="";
    int res = 0;

    chemin_ref.append(titre);
    chemin_ref.replace("plateau","reference");

    (*data)=allocation_m(chemin_ref,nb_passage,num_passage,cas,heure);
    errno_t err = fopen_s(&f,titre.toStdString().c_str(), "r+");

    if (err) {
        printf_s("The file fscanf.out was not opened\n");
        res = 1;
    }

    k = 0;
    incr = 0;

    while ( fgets(lecture, 100, f) && !sortie ) {


        if ( strcmp(lecture, "fin\n") == 0 ) {
                    incr ++;
        }
        else if ( incr == num_passage ) {
            sscanf(lecture,"%lf %lf %lf\n",&(*data)[0][k],&(*data)[1][k],&(*data)[2][k]);
            //prov. elim. bruit
            for(int o=0;o<3;o++) {
                if ( (*data)[o][k] > 0.5 ) {
                    (*data)[3][k] += (*data)[o][k];
                }
            }
            k++;

        }
        else if (incr > num_passage ){
            sortie = 1;
        }
    }
    *nb_valeur = k;
    chemin_ref.clear();
    free(lecture);

    fclose(f);
    return res;
}
