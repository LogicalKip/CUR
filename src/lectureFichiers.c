#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "../include/main.h"
#include "../include/constantes.h"
#include "../include/fonctionsBoucles.h"
#include "../include/autresFonctions.h"
#include "../include/fonctionsAnnexes.h"
#include "../include/lectureFichiers.h"
#include "../include/calculPrealable.h"



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


void affecterBonus(Carte *card, int type, int modificateur, int minimum)
{
    card->bonus.type = type;
    card->bonus.modificateur = modificateur;
    card->bonus.minimum = minimum;
}

void definirBonus(Carte *card, int compteur)
{
    if (strcmp(card->clan, "junta") == 0)
        affecterBonus(card, AUGMENTER_DEGATS, 2, 0);

    else if (strcmp(card->clan, "junkz") == 0)
        affecterBonus(card, AUGMENTER_ATTAQUE, 8, 0);

    else if (strcmp(card->clan, "pussycats") == 0)
        affecterBonus(card, DIMINUER_DEGATS, -2, 1);

    else if (strcmp(card->clan, "fangpi") == 0)
        affecterBonus(card, AUGMENTER_DEGATS, 2, 0);

    else if (strcmp(card->clan, "uppers") == 0)
        affecterBonus(card, DIMINUER_ATTAQUE, -10, 3);

    else if (strcmp(card->clan, "montana") == 0)
        affecterBonus(card, DIMINUER_ATTAQUE, -12, 8);

    else if (strcmp(card->clan, "sentinel") == 0)
        affecterBonus(card, AUGMENTER_ATTAQUE, 8, 0);

    else if (strcmp(card->clan, "sakrohm") == 0)
        affecterBonus(card, DIMINUER_ATTAQUE, -8, 3);

    else if (strcmp(card->clan, "skeelz") == 0)
        affecterBonus(card, PROTECTION_POUVOIR, 0, 0);

    else if (strcmp(card->clan, "gheist") == 0)
        affecterBonus(card, STOP_POUVOIR, 0, 0);

    else if (strcmp(card->clan, "roots") == 0)
        affecterBonus(card, STOP_POUVOIR, 0, 0);

    else if (strcmp(card->clan, "nightmare") == 0)
        affecterBonus(card, STOP_BONUS, 0, 0);

    else if (strcmp(card->clan, "piranas") == 0)
        affecterBonus(card, STOP_BONUS, 0, 0);

    else if (strcmp(card->clan, "allstars") == 0)
        affecterBonus(card, DIMINUER_PUISSANCE, -2, 1);

    else if (strcmp(card->clan, "bangers") == 0)
        affecterBonus(card, AUGMENTER_PUISSANCE, 2, 0);

    else if (strcmp(card->clan, "uluwatu") == 0)
        affecterBonus(card, AUGMENTER_PUISSANCE, 2, 0);

    else if (strcmp(card->clan, "freaks") == 0)
        affecterBonus(card, POISON, -2, 3);

    else if (strcmp(card->clan, "jungo") == 0)
        affecterBonus(card, GAIN_VIE, 2, 0);

    else if (strcmp(card->clan, "berzerk") == 0)
        affecterBonus(card, PERTE_VIE, -2, 2);

    else if (strcmp(card->clan, "rescue") == 0)
        affecterBonus(card, AUGMENTER_ATTAQUE, 3 * compteur, 0);

    else
        printf("\n%s : Clan encore inconnu. à définir dans lectureFichiers/definirBonus()\n", card->clan);
}

void miseEnPlaceDesBonus()
{
    miseEnPlaceDesBonusPourUnCamp(carteAlliee);
    miseEnPlaceDesBonusPourUnCamp(carteEnnemie);
}


void miseEnPlaceDesBonusPourUnCamp(Carte equipe[4])
{
    int i = 0, z = 0, compteur = 0;
    for (i = 0 ; i < 4 ; i++)
        {
            compteur = 0;

            for (z = 0 ; z < 4 ; z++)
                {//on compte combien ils sont du meme clan
                    if (strcmp(equipe[i].clan, equipe[z].clan) == 0)
                        compteur++;
                }

            if (compteur >= 2 && (strcmp(equipe[0].nom, equipe[1].nom) != 0 || strcmp(equipe[0].nom, equipe[2].nom) != 0 || strcmp(equipe[0].nom, equipe[3].nom ) != 0))
                definirBonus(&equipe[i], compteur);

            else
                {
                    equipe[i].bonus.type = NEANT;
                    equipe[i].bonus.modificateur = NEANT;
                    equipe[i].bonus.minimum = NEANT;
                }
        }
}

bool rechercheCarte(char nomCherche[], Carte *card)
{
    FILE* fichier = NULL;

    fichier = fopen("persos.txt", "r");

    if (fichier != NULL)
    {
        while (!feof(fichier))//il lit deux fois le dernier, c'est pas bien grave, mais comment y remedier?
        {
            if(fscanf(fichier, "%d %d %d %d %d %d %d %s %s", &card->pouvoir.type, &card->pouvoir.modificateur, &card->pouvoir.minimum, &card->pouvoir.condition, &card->puissance, &card->degatsDeBase, &card->nombreDEtoiles, card->nom, card->clan) != 9)
                {
                    printf("\nProbleme dans la liste de personnages.\n");
                    printf("Dernier personnage lu correctement : %s (%s)\n", card->nom, card->clan);
                    exit(0);
                }

            if (strcmp(card->nom, "Trinmkkt") == 0 || strcmp(card->nom, "Skrumxxt") == 0)
                card->nom[7] = 'T';

            else if (strcmp(card->nom, "Graksmxxt") == 0)//possiblité d'aller plus vite en prenant 10 lignes de plus.
                card->nom[8] = 'T';// TODO MAJ quand de nouveaux sakrhom ou autres persos aux noms abominables apparaissent

            if (strcmp(card->nom, nomCherche) == 0)
                {
                    printf("Personnage trouve : %s\n", card->nom);
                    fclose(fichier);
                    return true;
                }
        }
        fclose(fichier);

        printf("ERREUR : Personnage introuvable : %s.\nVerifiez l'orthographe et remplacez les espaces par des _ (underscores).\nExemples : No_Nam, Slyde, Tanaereva_Cr, TrinmkkT, Miss_Lulabee\n", nomCherche);
        return false;
    }

    else
    {//afficher erreur
        printf("Probleme lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void lectureDesFichiers()
{
    int i = 0;
    char nom[20] = "rien";

    for (i = 0 ; i < 4 ; i++)
    {
        do
        {
#ifdef EQUIPES_TEST
            if (i == 0)
                strcpy(nom, "wardog");
            if (i == 1)
                strcpy(nom, "thormund");
            if (i == 2)
                strcpy(nom, "chiro");
            if (i == 3)
                strcpy(nom, "no_nam");
#else
            if (i == 0)
                printf("\nQui est ta 1ere carte ? : ");
            else
                printf("\nQui est ta %deme carte ? : ", i + 1);

            scanf("%s", nom);
#endif

            changerMajuscules(nom);
        }
        while (!rechercheCarte(nom, &carteAlliee[i]));
    }

    printf("\n");

    for (i = 0 ; i < 4 ; i++)
    {
        do
        {
#ifdef EQUIPES_TEST
            if (i == 0)
                strcpy(nom, "kinjo");
            if (i == 1)
                strcpy(nom, "sakazuki");
            if (i == 2)
                strcpy(nom, "lewis");
            if (i == 3)
                strcpy(nom, "yu_mei");
#else
            if (i == 0)
                printf("\nQui est la 1ere carte adverse ? : ");
            else
                printf("\nQui est la %deme carte adverse ? : ", i + 1);

            scanf("%s", nom);
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
    int i = 0, z = 0, compteur = 0, compteurEnnemi = 0;

    for (i = 0 ; i < 4 ; i++)
        {
            compteur = 0;

            for (z = 0 ; z < 4 ; z++)
                {//on compte combien ils sont du meme clan
                    if (strcmp(carteAlliee[i].clan, carteAlliee[z].clan) == 0)//les clans sont les mêmes
                        compteur++;

                    if (strcmp(carteEnnemie[i].clan, carteEnnemie[z].clan) == 0)
                        compteurEnnemi++;
                }

            for (i = 0 ; i < 4 ; i++)
                {
                    if (carteAlliee[i].pouvoir.condition == SUPPORT)
                        carteAlliee[i].pouvoir.modificateur *= compteur;

                    if (carteEnnemie[i].pouvoir.condition == SUPPORT)
                        carteEnnemie[i].pouvoir.modificateur *= compteurEnnemi;
                }
        }
}


