#pragma once


#include <string>

#define CARDS_DB_FILENAME "persos.txt"
#define X_PREMIERS_ROUNDS_A_TESTER 2

#define EQUIPES_TEST //permet de générer deux équipes pour les besoins du debug
//#define NE_PAS_TRAITER_1ER_ROUND // permet de savoir s'il faut calculer le 1er round, ou demander ce qu'il s'est passé sans le traiter (pour des raisons de performance)


using std::string;

enum AbilityType {NOTHING, STOP_BONUS, STOP_POUVOIR, PROTECTION_POUVOIR, PROTECTION_BONUS,
        AUGMENTER_PUISSANCE, DIMINUER_PUISSANCE, AUGMENTER_DEGATS, DIMINUER_DEGATS, AUGMENTER_ATTAQUE, DIMINUER_ATTAQUE, POISON, GAIN_VIE, PERTE_VIE, GAIN_PILLZ, PERTE_PILLZ, COPIER_DEGATS, COPIER_PUISSANCE, PROTECTION_PUISSANCE, PROTECTION_DEGATS};
//TODO fixit, recup pillz, pv/degats, copie pouvoir et bonus, tameshi, cybil...
enum AbilityCondition {NO_CONDITION, SUPPORT, EN_CAS_DE_STOP, COURAGE, REVANCHE, CONFIANCE, DEFAITE, CONTRECOUP};

enum DuelResult {NOT_YET_KNOWN, VAINQUEUR, PERDANT, EGALITE};



typedef struct CardAbility CardAbility;
struct CardAbility
{
    int minimum;
    int modificateur;
    AbilityType type;
    AbilityCondition condition;
};


typedef struct Carte Carte;
struct Carte
{
    CardAbility pouvoir;
    CardAbility bonus;
    int puissance;
    int puissanceTemporaire;
    int degatsDeBase;
    bool supposeeUtilisee;
    bool utiliseeACoupSur;
    int nombreDEtoiles;
    int degatsContre[4];
    bool pouvoirEstActifContre[4];
    bool bonusEstActifContre[4];
    int degatsAvecFuryContre[4];
    int victoiresAvecXpillzEntreCrochets[13]; // et si il y a plus de 12 pillz?
    int victoiresAvecXpillzEtFury[10];        // idem
    int defaitesAvecXpillz[13];             //TODO une constante pour remplacer 13 et 10
    int defaitesAvecXpillzEtFury[10];
    int egalitesAvecXPillz[13];
    int egalitesAvecXPillzEtFury[10];
    DuelResult combatAvecXPillzContreYAvecZpillz[13][4][13];
    int guessedScore[13] = {0};//FIXME still here when not using it : problem ?
    int guessedScoreFury[10] = {0};
    string nom;
    string clan;
};


const string ALREADY_PLAYED_STRING = "This card has already been played !";
const char YES_CHAR = 'y';
const char NO_CHAR  = 'n';  
