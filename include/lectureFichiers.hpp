#pragma once


#include "constantes.hpp"
#include "fonctionsBoucles.hpp"
#include "autresFonctions.hpp"



using std::string;


char equivalentMinuscule(char lettreRecue);

char equivalentMajuscule(char lettreRecue);

void miseEnPlaceDesBonus();

void miseEnPlaceDesBonusPourUnCamp(Carte equipe[4]);

void lectureDesFichiers();

bool rechercheCarte(string nomCherche, Carte * card);

void multiplicationParSupport();

void definirBonus(Carte *card, int compteur);

void affecterBonus(Carte *card, AbilityType type, int modificateur, int minimum);

AbilityType stringToAbilityType(string s);

AbilityCondition stringToAbilityCondition(string s);