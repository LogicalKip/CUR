#pragma once

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

string whatAboutPillzHP(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses);

void whatAboutPersos(Carte cards[4]);

void affichageDesVictoires();

void quiCommenceSelonLesEtoiles();

void erreur(string message);

void whatAboutAbility(CardAbility pvr);

