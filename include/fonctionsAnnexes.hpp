#pragma once

#include <string>

#include "constantes.hpp"
#include "fonctionsBoucles.hpp"
#include "autresFonctions.hpp"

using std::string;

void normalizeCasing(string& nom);

void testFinDeJeu(int pointsDeVieAdverses, int pointsDeVie);

void effacerEcran();

void reset();

void whatHappenedRound1(int *pillz, int *pillzAdverses, int *pointsDeVie, int *pointsDeVieAdverses);

string whatAboutPillzHP(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses);

void whatAboutCards(Carte cards[4]);

void displayComputedStats(int pillzRestant);

void findOutFirstPlayer();

void erreur(string message);

void whatAboutAbility(CardAbility pvr);

