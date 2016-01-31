

#ifndef DEF_ANNEXES
#define DEF_ANNEXES

#include <string>

#include "constantes.hpp"
#include "fonctionsBoucles.hpp"
#include "autresFonctions.hpp"

using std::string;

void changerMajuscules(string& nom);

void testFinDeJeu(int pointsDeVieAdverses, int pointsDeVie);

void effacerEcran();

void remiseAZero();

void whatHappenedRound1(int *pillz, int *pillzAdverses, int *pointsDeVie, int *pointsDeVieAdverses);

std::string whatAboutPillzHP(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses);

void whatAboutPersos(Carte cards[4]);

void affichageDesVictoires();

void quiCommenceSelonLesEtoiles();

void erreur(string message);

void whatAboutPvr(PouvoirDeCarte pvr);

void whatAboutbn(BonusDeCarte bn);



#endif

