#pragma once


void findOutWhichCardEnemySent(string question, int round, int& enemyCardSent);

string getIdleReminder(Carte deck[4]);

int getLastIdleCard(Carte deck[4]);

void quit();

string getOrdinal(int n);

int carteEnvoyeeParLEnnemiEnCourage(int *carteQuIlEnvoie, int round);

void faisCa(int *pillz, int *pillzQueJUtilise, int *carteQueJEnvoie, bool *ilFautUtiliserLaFury);

void reponseEnnemie(int *pillzAdverses, int *pillzQuIlUtilise, int *carteQuIlEnvoie, bool *ennemiUtiliseFury, int round);

void ilAvaitMisCombienDePillz(int pillzAdverses, int *pillzQuIlUtilise, bool *ennemiUtiliseFury);

void majPvPillz(int *pointsDeVie, int *pointsDeVieAdverses, int *pillz, int *pillzAdverses, int carteEnvoyeeParEnnemi, int carteQueJEnvoie, int pillzUtilisesParEnnemi, int pillzQueJUtilise, int round, int ilFautUtiliserLaFury, int furyEnnemie);

void processGame(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses);


void gererPremierRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round);

void traiterRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round, bool courageEnnemi, int dernierRoundACalculer);
