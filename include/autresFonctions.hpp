
#ifndef DEF_AUTRESFONCTIONS
#define DEF_AUTRESFONCTIONS


int carteEnvoyeeParLEnnemiEnCourage(int *carteQuIlEnvoie, int round);

void faisCa(int *pillz, int *pillzQueJUtilise, int *carteQueJEnvoie, bool *ilFautUtiliserLaFury);

void reponseEnnemie(int *pillzAdverses, int *pillzQuIlUtilise, int *carteQuIlEnvoie, bool *ennemiUtiliseFury, int round);

void ilAvaitMisCombienDePillz(int pillzAdverses, int *pillzQuIlUtilise, bool *ennemiUtiliseFury);

void majPvPillz(int *pointsDeVie, int *pointsDeVieAdverses, int *pillz, int *pillzAdverses, int carteEnvoyeeParEnnemi, int carteQueJEnvoie, int pillzUtilisesParEnnemi, int pillzQueJUtilise, int round, int ilFautUtiliserLaFury, int furyEnnemie);

void cestLEnnemiQuiCommence(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses);

void cestMoiQuiCommence(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses);

void gererPremierRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round);

void traiterRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round, bool courageEnnemi, int dernierRoundACalculer);


#endif

