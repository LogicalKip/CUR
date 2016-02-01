


#include <iostream>
#include <fstream>



#include "../include/main.hpp"
#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/lectureFichiers.hpp"
#include "../include/calculPrealable.hpp"



using std::cin;
using std::cout;
using std::endl;
using std::ifstream;


char equivalentMajuscule(char lettreRecue)
{
    if (lettreRecue >= 'a' && lettreRecue <= 'z')
        return lettreRecue - ('a' - 'A');
    else
        return lettreRecue;
}

char equivalentMinuscule(char lettreRecue)
{
    if (lettreRecue >= 'A' && lettreRecue <= 'Z')
        return lettreRecue + ('a' - 'A');
    else
        return lettreRecue;
}


void affecterBonus(Carte *card, AbilityType type, int modificateur, int minimum)
{
    card->bonus.type = type;
    card->bonus.modificateur = modificateur;
    card->bonus.minimum = minimum;
    card->bonus.condition = NO_CONDITION;
}

void definirBonus(Carte *card, int compteur)
{
    if (card->clan == "junta")
        affecterBonus(card, AUGMENTER_DEGATS, 2, 0);

    else if (card->clan == "junkz")
        affecterBonus(card, AUGMENTER_ATTAQUE, 8, 0);

    else if (card->clan == "pussycats")
        affecterBonus(card, DIMINUER_DEGATS, -2, 1);

    else if (card->clan == "fangpi")
        affecterBonus(card, AUGMENTER_DEGATS, 2, 0);

    else if (card->clan == "uppers")
        affecterBonus(card, DIMINUER_ATTAQUE, -10, 3);

    else if (card->clan == "montana")
        affecterBonus(card, DIMINUER_ATTAQUE, -12, 8);

    else if (card->clan == "sentinel")
        affecterBonus(card, AUGMENTER_ATTAQUE, 8, 0);

    else if (card->clan == "sakrohm")
        affecterBonus(card, DIMINUER_ATTAQUE, -8, 3);

    else if (card->clan == "skeelz")
        affecterBonus(card, PROTECTION_POUVOIR, 0, 0);

    else if (card->clan == "gheist")
        affecterBonus(card, STOP_POUVOIR, 0, 0);

    else if (card->clan == "roots")
        affecterBonus(card, STOP_POUVOIR, 0, 0);

    else if (card->clan == "nightmare")
        affecterBonus(card, STOP_BONUS, 0, 0);

    else if (card->clan == "piranas")
        affecterBonus(card, STOP_BONUS, 0, 0);

    else if (card->clan == "allstars")
        affecterBonus(card, DIMINUER_PUISSANCE, -2, 1);

    else if (card->clan == "bangers")
        affecterBonus(card, AUGMENTER_PUISSANCE, 2, 0);

    else if (card->clan == "uluwatu")
        affecterBonus(card, AUGMENTER_PUISSANCE, 2, 0);

    else if (card->clan == "freaks")
        affecterBonus(card, POISON, -2, 3);

    else if (card->clan == "jungo")
        affecterBonus(card, GAIN_VIE, 2, 0);

    else if (card->clan == "berzerk")
        affecterBonus(card, PERTE_VIE, -2, 2);

    else if (card->clan == "rescue")
        affecterBonus(card, AUGMENTER_ATTAQUE, 3 * compteur, 0);

    else
        erreur(card->clan + " : Clan is unknown. Must be defined in lectureFichiers/definirBonus()\n");
}

void miseEnPlaceDesBonus()
{
    miseEnPlaceDesBonusPourUnCamp(carteAlliee);
    miseEnPlaceDesBonusPourUnCamp(carteEnnemie);
}


void miseEnPlaceDesBonusPourUnCamp(Carte equipe[4])
{
    int compteur = 0;
    for (int i = 0 ; i < 4 ; i++)
    {
        compteur = 0;

        for (int z = 0 ; z < 4 ; z++)
        {//on compte combien ils sont du meme clan
            if (equipe[i].clan == equipe[z].clan)
                compteur++;
        }

        if (compteur >= 2 && (equipe[0].nom != equipe[1].nom || equipe[0].nom != equipe[2].nom || equipe[0].nom != equipe[3].nom))
            definirBonus(&equipe[i], compteur);

        else
        {
            equipe[i].bonus.type = NOTHING;
            equipe[i].bonus.modificateur = 0;
            equipe[i].bonus.minimum = 0;
        }
    }
}

AbilityType stringToAbilityType(string s) 
{
    AbilityType res;

    if (s == "NOTHING")
        res = NOTHING;
    else if (s == "STOP_BONUS")
        res = STOP_BONUS;
    else if (s == "STOP_POUVOIR")
        res = STOP_POUVOIR;
    else if (s == "PROTECTION_POUVOIR")
        res = PROTECTION_POUVOIR;
    else if (s == "PROTECTION_BONUS")
        res = PROTECTION_BONUS;
    else if (s == "AUGMENTER_PUISSANCE")
        res = AUGMENTER_PUISSANCE;
    else if (s == "DIMINUER_PUISSANCE")
        res = DIMINUER_PUISSANCE;
    else if (s == "AUGMENTER_DEGATS")
        res = AUGMENTER_DEGATS;
    else if (s == "DIMINUER_DEGATS")
        res = DIMINUER_DEGATS;
    else if (s == "AUGMENTER_ATTAQUE")
        res = AUGMENTER_ATTAQUE;
    else if (s == "DIMINUER_ATTAQUE")
        res = DIMINUER_ATTAQUE;
    else if (s == "POISON")
        res = POISON;
    else if (s == "GAIN_VIE")
        res = GAIN_VIE;
    else if (s == "PERTE_VIE")
        res = PERTE_VIE;
    else if (s == "GAIN_PILLZ")
        res = GAIN_PILLZ;
    else if (s == "PERTE_PILLZ")
        res = PERTE_PILLZ;
    else if (s == "COPIER_DEGATS")
        res = COPIER_DEGATS;
    else if (s == "COPIER_PUISSANCE")
        res = COPIER_PUISSANCE;
    else if (s == "PROTECTION_PUISSANCE")
        res = PROTECTION_PUISSANCE;
    else if (s == "PROTECTION_DEGATS")
        res = PROTECTION_DEGATS;
    else {
        res = NOTHING;
        erreur(s + " is not known as any ability type");
    }
    return res;
}


AbilityCondition stringToAbilityCondition(string s) 
{
    AbilityCondition res;
    if (s == "NO_CONDITION")
        res = NO_CONDITION;
    else if (s == "SUPPORT")
        res = SUPPORT;
    else if (s == "EN_CAS_DE_STOP")
        res = EN_CAS_DE_STOP;
    else if (s == "COURAGE")
        res = COURAGE;
    else if (s == "REVANCHE")
        res = REVANCHE;
    else if (s == "CONFIANCE")
        res = CONFIANCE;
    else if (s == "DEFAITE")
        res = DEFAITE;
    else if (s == "CONTRECOUP")
        res = CONTRECOUP;
    else {
        erreur(s + " is not known as any condition type");
        res = NO_CONDITION;
    } 
    return res;
}

bool rechercheCarte(string nomCherche, Carte *card)
{
    bool cardFound = false;
    ifstream file(CARDS_DB_FILENAME);//FIXME need to close/whatever the filestream ?

    if (file.bad() || file.fail() || ! file.good())
    {
        cout << "There was a problem opening " << CARDS_DB_FILENAME << "." << endl;
        exit(EXIT_FAILURE);
    }

    int pMod, pMin, puissance, degats, etoiles;
    string nom, clan, pType, pCond;

    while (file >> pType >> pMod >> pMin >> pCond >> puissance >> degats >> etoiles >> nom >> clan)
    {
        if (nom == "Trinmkkt" || nom == "Skrumxxt")
            nom[7] = 'T';

        else if (nom == "Graksmxxt")//possiblité d'aller plus vite en prenant 10 lignes de plus.
            nom[8] = 'T';// TODO MAJ quand de nouveaux sakrhom ou autres persos aux noms abominables apparaissent

        if (nom == nomCherche)
        {
            cout << "Card found : " << nom << endl;

            card->pouvoir.type = stringToAbilityType(pType);
            card->pouvoir.modificateur = pMod;
            card->pouvoir.minimum = pMin;
            card->pouvoir.condition = stringToAbilityCondition(pCond);
            card->puissance = puissance;
            card->degatsDeBase = degats;
            card->nombreDEtoiles = etoiles;
            card->nom = nom;
            card->clan = clan;

            cardFound = true;
            break;
        }
    }

    if (! cardFound)
    {
        cout << "ERROR : cannot find the card : " << nomCherche << "." << endl << 
        "Check for spelling and replace spaces with underscores (_)." << endl << 
        "Examples : Tanaereva_Cr, No_Nam, Slyde, TrinmkkT, Miss_Lulabee" << endl;
       
    }
    return cardFound;
}

void lectureDesFichiers()
{
    string nom;

    for (int i = 0 ; i < 4 ; i++)
    {
        do
        {
#ifdef EQUIPES_TEST
            if (i == 0)
                nom = "gibson";
            if (i == 1)
                nom = "thormund";
            if (i == 2)
                nom = "chiro";
            if (i == 3)
                nom = "no_nam";
#else
            if (i == 0)
                cout << "\nQui est ta 1ere carte ? : ";
            else
                cout << "\nQui est ta " << i + 1 << "eme carte ? : ";

            cin >> nom;
#endif

            changerMajuscules(nom);
        }
        while (!rechercheCarte(nom, &carteAlliee[i]));
    }

    cout << "\n";

    for (int i = 0 ; i < 4 ; i++)
    {
        do
        {
#ifdef EQUIPES_TEST
            if (i == 0)
                nom = "kinjo";
            if (i == 1)
                nom = "sakazuki";
            if (i == 2)
                nom = "lewis";
            if (i == 3)
                nom = "yu_mei";
#else
            if (i == 0)
                cout << "\nQui est la 1ere carte adverse ? : ";
            else
                cout << "\nQui est la " << i + 1 << "eme carte adverse ? : ";

            cin >> nom;
#endif
            changerMajuscules(nom);
        }
        while (!rechercheCarte(nom, &carteEnnemie[i]));
    }

    miseEnPlaceDesBonus();

    multiplicationParSupport();
}

void multiplicationParSupport()
{
    for (int i = 0 ; i < 4 ; i++)
    {
        int compteur = 0, compteurEnnemi = 0;

        for (int z = 0 ; z < 4 ; z++)
        {//on compte combien ils sont du meme clan
            if (carteAlliee[i].clan == carteAlliee[z].clan)
                compteur++;

            if (carteEnnemie[i].clan == carteEnnemie[z].clan)
                compteurEnnemi++;
        }

        if (carteAlliee[i].pouvoir.condition == SUPPORT)
            carteAlliee[i].pouvoir.modificateur *= compteur;

        if (carteEnnemie[i].pouvoir.condition == SUPPORT)
            carteEnnemie[i].pouvoir.modificateur *= compteurEnnemi;
    }
}


