
#ifndef DEF_LECTURE
#define DEF_LECTURE


#include "constantes.hpp"
#include "fonctionsBoucles.hpp"
#include "autresFonctions.hpp"


char equivalentMinuscule(char lettreRecue);

char equivalentMajuscule(char lettreRecue);

void miseEnPlaceDesBonus();

void miseEnPlaceDesBonusPourUnCamp(Carte equipe[4]);

void lectureDesFichiers();

bool rechercheCarte(char nomCherche[], Carte * card);

void multiplicationParSupport();

void definirBonus(Carte *card, int compteur);

void affecterBonus(Carte *card, int type, int modificateur, int minimum);

#endif

