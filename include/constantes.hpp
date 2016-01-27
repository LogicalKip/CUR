
#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#include <stdbool.h>


#define X_PREMIERS_ROUNDS_A_TESTER 2

//#define EQUIPES_TEST //permet de générer deux équipes pour les besoins du debug
//#define NE_PAS_TRAITER_1ER_ROUND // permet de savoir s'il faut calculer le 1er round, ou demander ce qu'il s'est passé sans le traiter (pour des raisons de performance)

enum {NEANT, STOP_BONUS, STOP_POUVOIR, PROTECTION_POUVOIR, PROTECTION_BONUS,
        AUGMENTER_PUISSANCE, DIMINUER_PUISSANCE, AUGMENTER_DEGATS, DIMINUER_DEGATS, AUGMENTER_ATTAQUE, DIMINUER_ATTAQUE,
        VAINQUEUR, PERDANT, EGALITE, SUPPORT, POISON, GAIN_VIE, PERTE_VIE, GAIN_PILLZ, PERTE_PILLZ, EN_CAS_DE_STOP, COURAGE,
        REVANCHE, CONFIANCE, COPIER_DEGATS, COPIER_PUISSANCE, DEFAITE, CONTRECOUP, PROTECTION_PUISSANCE, PROTECTION_DEGATS};//fixit, recup pillz, pv/degats, copie pouvoir et bonus...
//FIXME typer l'enum et ses utilisations

typedef struct BonusDeCarte BonusDeCarte;
struct BonusDeCarte
{
    int minimum;
    int modificateur;
    int type;
};



typedef struct PouvoirDeCarte PouvoirDeCarte;
struct PouvoirDeCarte
{
    int minimum;
    int modificateur;
    int type;
    int condition;
};


typedef struct Carte Carte;
struct Carte
{
    PouvoirDeCarte pouvoir;
    BonusDeCarte bonus;
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
    int combatAvecXPillzContreYAvecZpillz[13][4][13];
    int guessedScore[13] = {0};
    int guessedScoreFury[10] = {0};
    char nom[20];
    char clan[15];// FIXME 10 suffiraient? la flemme de tous les compter...
};


#endif


