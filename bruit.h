#ifndef BRUIT_H
#define BRUIT_H

#include "qdebug.h"
#include "math.h"

#define BARRE_M 1.0

typedef struct Ensemblepic{
    double MoyBassePic;
    double MoyHautePic;

    double VarBassePic;
    double VarHautePic;

    double MoyDerivCroiPic;
    double MoyDerivDecrPic;

    double VarDerivCroiPic;
    double VarDerivDecrPic;

    double MaxPic;
    double MinPic;

    int NbPicHaut;
    int NbPicBas;

    int IndiceFinPic;
    int LgPic;

}Pic;

class bruit
{
public:
    bruit();

    Pic MoyPic(double** data, int k, int i, int NbAnalyse);

    void affichageInfoBruit(Pic PicRef);

    void InitRefPic(Pic *RefPic);
    void CopyPic(Pic *RefPic,Pic RefPictmp);
    void MiseAJourRefPic(Pic *RefPic, Pic tmp, Pic Tranch, int NbMiseAJour);
};

#endif // BRUIT_H
