
#ifndef DEF_CALCUL
#define DEF_CALCUL


#include "constantes.h"
#include "fonctionsBoucles.h"
#include "autresFonctions.h"

bool pouvoirActif(Carte carteATester, Carte adversairee);

bool bonusActif(Carte carteATester, Carte adversaire);

void calculDesDegats();

void calculDesDuels(int pillz, int pillzAdverses);

void augmenterPuissance(Carte *carteATester, Carte adversaire, int numAdversaire);

void diminuerPuissance(Carte carteATester, Carte *adversaire, int numCarteATester, int numAdversaire);

void modifierAttaque(int *attaque, Carte carteAModifier, Carte carteAffrontee, int numCarteAModifier, int numAdversaire);

void determinerDegats(Carte *carteATester, Carte adversaire, int numCarteATester, int numAdversaire);


#endif
