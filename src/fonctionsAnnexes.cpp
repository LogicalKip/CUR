#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <sstream>

#include "../include/main.hpp"
#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"


#if defined (WIN32)
#include <windows.h>
void effacerEcran()
{ system("CLS"); }
#elif defined (__linux)
void effacerEcran()
{ system("clear"); }
#endif

void erreur(char message[])
{
    printf("\n\nErreur dans l'écriture du programme : %s\nAppuyez sur Entrée pour quitter.\n", message);
    getchar();
    exit(0);
}

void changerMajuscules(char nom[])
{
    int i;
    for (i = 0 ; i < strlen(nom) ; i++)
        nom[i] = equivalentMinuscule(nom[i]);

    for (i = 0 ; i < strlen(nom) ; i++)
        {
            if (nom[i] == '_' && nom[i+1] != '\0')// Même si, a priori, aucun nom de finira par \0...
                nom[i+1] = equivalentMajuscule(nom[i+1]);
        }

    nom[0] = equivalentMajuscule(nom[0]);
}

void whatHappenedRound1(int *pillz, int *pillzAdverses, int *pointsDeVie, int *pointsDeVieAdverses)
{//mMhmmhm... peut-etre changer l'ordre des questions en fonction de qui commence, pour paraître plus logique...
    int carteEnvoyeeEnnemie = 0, pillzUtilisesParLEnnemi = 0, carteEnvoyeeAlliee = 0, pillzUtilisesAllies = 0, i = 0;
    bool ilUtiliseFury = false, jUtiliseFury = false, nomInexistant = false;
    char nomEnnemi[20] = "rien";
    char nomAllie[20] = "aussi";

    do
    {//qui l'ennemi a-t-il envoye?
        printf("\nQuelle carte a envoye ton adversaire au premier round ? ");
        scanf("%s", nomEnnemi);

        changerMajuscules(nomEnnemi);

        for (i = 0 ; i < 4 ; i++)
        {
            if (strcmp(carteEnnemie[i].nom, nomEnnemi) == 0)
                {
                    carteEnvoyeeEnnemie = i;
                    nomInexistant = false;
                    break;
                }

            else
                nomInexistant = true;
        }

        if (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || nomInexistant)
            printf("\nEuh.... Ou pas?\n");
    }
    while (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || nomInexistant);

    carteEnnemie[carteEnvoyeeEnnemie].utiliseeACoupSur = 1;

    ilAvaitMisCombienDePillz(*pillzAdverses, &pillzUtilisesParLEnnemi, &ilUtiliseFury);

    do
    {//qui l'utilisateur a-t-il envoye?
        printf("\n\nEt toi, tu as envoye quelle carte au premier round ? ");
        scanf("%s", nomAllie);

        changerMajuscules(nomAllie);

        for (i = 0 ; i < 4 ; i++)
        {
            if (strcmp(carteAlliee[i].nom, nomAllie) == 0)
                {
                    carteEnvoyeeAlliee = i;
                    nomInexistant = false;
                    break;
                }

            else
                nomInexistant = true;
        }

        if (carteEnvoyeeAlliee < 0 || carteEnvoyeeAlliee > 3 || nomInexistant)
            printf("\nNon, franchement...\n");
    }
    while (carteEnvoyeeAlliee < 0 || carteEnvoyeeAlliee > 3 || nomInexistant);

    carteAlliee[carteEnvoyeeAlliee].utiliseeACoupSur = 1;



    ilAvaitMisCombienDePillz(*pillz, &pillzUtilisesAllies, &jUtiliseFury);// malgré le nom, on peut quand même utiliser cette fonction pour moi... non ?

    //----------------------Quelqu'un a donc perdu des pv (et surement aussi des pillzs) !------------------//

    *pillz -= pillzUtilisesAllies;
    *pillzAdverses -= pillzUtilisesParLEnnemi;

    if (jUtiliseFury)
        *pillz -= 3;

    if (ilUtiliseFury)
        *pillzAdverses -= 3;

    if (carteAlliee[carteEnvoyeeAlliee].combatAvecXPillzContreYAvecZpillz[pillzUtilisesAllies][carteEnvoyeeEnnemie][pillzUtilisesParLEnnemi] == VAINQUEUR)
        {//si j'ai eu plus d'attaque que lui
            modifPerteGainVie(carteEnvoyeeAlliee, pointsDeVie, carteAlliee[carteEnvoyeeAlliee], carteEnvoyeeEnnemie, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeEnnemie], jUtiliseFury);
            modifPerteGainPillz(carteEnvoyeeAlliee, pillz, carteAlliee[carteEnvoyeeAlliee], carteEnvoyeeEnnemie, pillzAdverses, carteEnnemie[carteEnvoyeeEnnemie]);
        }

    else if (carteAlliee[carteEnvoyeeAlliee].combatAvecXPillzContreYAvecZpillz[pillzUtilisesAllies][carteEnvoyeeEnnemie][pillzUtilisesParLEnnemi] == PERDANT)
        {//si j'ai eu moins d'attaque que lui
            modifPerteGainVie(carteEnvoyeeEnnemie, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeEnnemie], carteEnvoyeeAlliee, pointsDeVie, carteAlliee[carteEnvoyeeAlliee], ilUtiliseFury);
            modifPerteGainPillz(carteEnvoyeeEnnemie, pillzAdverses, carteEnnemie[carteEnvoyeeEnnemie], carteEnvoyeeAlliee, pillz, carteAlliee[carteEnvoyeeAlliee]);
        }

    else if (carteAlliee[carteEnvoyeeAlliee].combatAvecXPillzContreYAvecZpillz[pillzUtilisesAllies][carteEnvoyeeEnnemie][pillzUtilisesParLEnnemi] == EGALITE)
        {//egalite d'attaque et d'étoiles...
            if (egaliteDAttaque(1))//return 1 : j'ai gagne     return 0 : j'ai perdu
                {
                    modifPerteGainVie(carteEnvoyeeAlliee, pointsDeVie, carteAlliee[carteEnvoyeeAlliee], carteEnvoyeeEnnemie, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeEnnemie], jUtiliseFury);
                    modifPerteGainPillz(carteEnvoyeeAlliee, pillz, carteAlliee[carteEnvoyeeAlliee], carteEnvoyeeEnnemie, pillzAdverses, carteEnnemie[carteEnvoyeeEnnemie]);
                }

            else
                {
                    modifPerteGainVie(carteEnvoyeeEnnemie, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeEnnemie], carteEnvoyeeAlliee, pointsDeVie, carteAlliee[carteEnvoyeeAlliee], ilUtiliseFury);
                    modifPerteGainPillz(carteEnvoyeeEnnemie, pillzAdverses, carteEnnemie[carteEnvoyeeEnnemie], carteEnvoyeeAlliee, pillz, carteAlliee[carteEnvoyeeAlliee]);
                }
        }

    else
        erreur("fonctionsAnnexes/whatHappenedRound1 : combat ne vaut ni VAINQUEUR, ni PERDANT, ni EGALITE\n");
}

void testFinDeJeu(int pointsDeVieAdverses, int pointsDeVie)
{
    if (pointsDeVie < 1)
        {
            printf("\nDesole... J'ai fait ce que j'ai pu, t'as qu'a t'en prendre au destin...\n\n\n...ou au programmeur ^^.\n\nN'hesite pas a me re-utiliser, ca marchera peut-etre mieux la prochaine fois !\n\n\nAppuie sur Entree pour quitter\n");
            getchar();
            exit(EXIT_SUCCESS);// enfin... façon de parler...
        }

    if (pointsDeVieAdverses < 1)
        {
            #if defined (WIN32)//petit cadeau pour les windowsiens vainqueurs ;)
            Beep(630, 150);
            Beep(630, 150);
            Beep(630, 150);
            Beep(630, 450);
            Beep(500, 500);
            Beep(561, 400);
            Beep(630, 300);
            Beep(561, 150);
            Beep(630, 525);
            #endif
            printf("\nOUAIIIIS ! C'est nous qu'on est les meilleurs ! Allez salut !\n\n\n\nAppuie sur Entree pour quitter\n");
            getchar();
            exit(EXIT_SUCCESS);
        }
}

void remiseAZero()
{
    int i = 0, j = 0;

    for (i = 0 ; i < 4 ; i++)
        {// aucune carte n'est utilisee !! Ou alors ca se saura grâce a utiliseeACoupSur. Ici ce sont les variables utilisees dans les boucles (qui servent a eviter de calculer une carte dont on considere qu'elle a ete utilisee lors des rounds precedents)
            carteAlliee[i].supposeeUtilisee = false;
            carteEnnemie[i].supposeeUtilisee = false;
        }

    for (j = 0 ; j < 4 ; j++)//mise a 0 des victoires
        {
            for (i = 0 ; i <= 12 ; i++)//condition de fin de boucle a changer?
                {
                    carteAlliee[j].victoiresAvecXpillzEntreCrochets[i] = 0;
                    carteAlliee[j].defaitesAvecXpillz[i] = 0;
                    carteAlliee[j].egalitesAvecXPillz[i] = 0;
                }
        }
    ///Inutile de reset les ennemis, car on ne se sert pas de leurs nombresDeVictoires
    for (j = 0 ; j < 4 ; j++)//mise a 0 des victoires
        {
            for (i = 0 ; i <= 9 ; i++)//condition de fin de boucle a changer?
                {
                    carteAlliee[j].victoiresAvecXpillzEtFury[i] = 0;
                    carteAlliee[j].defaitesAvecXpillzEtFury[i] = 0;
                    carteAlliee[j].egalitesAvecXPillzEtFury[i] = 0;
                }
        }

    persoTest = 0;
    pillzTest = 0;
    furyUtilisee = false;
}

std::string whatAboutPillzHP(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses)
{
	std::stringstream res;
	
	res << "\n\nToi :\n\tPoints de vie : " << pointsDeVie << "\n\tPillz : " << pillz << "\n\nTon ennemi :\n\tPoints de vie : " << pointsDeVieAdverses << "\n\tPillz : " << pillzAdverses << "\n\n";
	
	return res.str();
}

void affichageDesVictoires()
{
    int i = 0, j = 0;
    char afficher = 0;

    printf("\nAfficher les victoires ? (o/n) : ");

    while(afficher != 'o' && afficher != 'n')
       scanf("%c", &afficher);

    if (afficher == 'o')
    {
        printf("\n\n\n");
        for (j = 0 ; j < 4 ; j++)
            {//affichage des victoires histoire de verifier
                if (! carteAlliee[j].utiliseeACoupSur)
                    {
                        printf("Victoires de %s :\n", carteAlliee[j].nom);

                        for (i = 0 ; i <= 12 ; i++)
                        {
                            printf("\t");
                            if (i < 10)
                                printf(" ");

                            printf("%d pillz : V=%d E=%d D=%d", i, carteAlliee[j].victoiresAvecXpillzEntreCrochets[i], carteAlliee[j].egalitesAvecXPillz[i], carteAlliee[j].defaitesAvecXpillz[i]);

                            if (i <= 9)
                                printf("\n\t %d pillz et fury : V=%d E=%d D=%d", i, carteAlliee[j].victoiresAvecXpillzEtFury[i], carteAlliee[j].egalitesAvecXPillzEtFury[i], carteAlliee[j].defaitesAvecXpillzEtFury[i]);

                            printf("\n\n");
                        }
                        printf("\n\n\n");
                    }
            }
    }
}

void quiCommenceSelonLesEtoiles()
{
    int mesEtoiles = carteAlliee[0].nombreDEtoiles + carteAlliee[1].nombreDEtoiles + carteAlliee[2].nombreDEtoiles + carteAlliee[3].nombreDEtoiles;
    int sesEtoiles = carteEnnemie[0].nombreDEtoiles + carteEnnemie[1].nombreDEtoiles + carteEnnemie[2].nombreDEtoiles + carteEnnemie[3].nombreDEtoiles;
    int repJoueur = 5;
    if (sesEtoiles > mesEtoiles)
        ennemiCommence = true;//si il a plus d'etoiles que moi, il commence.

    else if (sesEtoiles < mesEtoiles)
        ennemiCommence = false;//il a moins d'etoiles que moi : je commence

    else
        {//chouette, on a autant d'etoiles...
            do
            {//mais alors qui a commencé?
                printf("Qui commence ? 1 si c'est ton ennemi qui commence, 0 si c'est toi\n");
                scanf("%d", &repJoueur);

                if (repJoueur != 0 && repJoueur != 1)
                    printf("\nAh, ah, tres drole...\n");
            }while (repJoueur != 0 && repJoueur != 1);

            ennemiCommence = (repJoueur == 1);
        }
}

void whatAboutPvr(PouvoirDeCarte pvr)
{
    if (pvr.condition == SUPPORT)
        printf("(support) ");

    if (pvr.condition == COURAGE)
        printf("Courage : ");

    if (pvr.condition == CONFIANCE)
        printf("Confiance : ");

    if (pvr.condition == REVANCHE)
        printf("Revanche : ");

    if (pvr.condition == EN_CAS_DE_STOP)
        printf("Stop : ");

    if (pvr.condition == DEFAITE)
        printf("Defaite : ");

    if (pvr.type == AUGMENTER_ATTAQUE)
        printf("+%d a l'attaque", pvr.modificateur);

    if (pvr.type == DIMINUER_ATTAQUE)
        printf("%d a l'attaque adverse, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == AUGMENTER_PUISSANCE)
        printf("+%d a la puissance", pvr.modificateur);

    if (pvr.type == DIMINUER_PUISSANCE)
        printf("%d a la puissance adverse, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == AUGMENTER_DEGATS)
        printf("+%d aux degats", pvr.modificateur);

    if (pvr.type == COPIER_PUISSANCE)
        printf("Puissance = puissance adverse");

    if (pvr.type == COPIER_DEGATS)
        printf("Degats = Degats adverses");

    if (pvr.type == DIMINUER_DEGATS)
        printf("%d aux degats adverses, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == PROTECTION_BONUS)
        printf("Protection bonus");

    if (pvr.type == STOP_BONUS)
        printf("Stop bonus");

    if (pvr.type == STOP_POUVOIR)
        printf("Stop pouvoir");

    if (pvr.type == PERTE_PILLZ)
        printf("%d pillz, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == PERTE_VIE)
        printf("%d vie, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == GAIN_PILLZ)
        printf("+%d pillz", pvr.modificateur);

    if (pvr.type == GAIN_VIE)
        printf("+%d vie", pvr.modificateur);

    if (pvr.type == POISON)
        printf("Poison %d, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == NEANT)
        printf("Aucun");
}

void whatAboutbn(BonusDeCarte bn)
{
    if (bn.type == AUGMENTER_ATTAQUE)
        printf("+%d a l'attaque", bn.modificateur);

    if (bn.type == DIMINUER_ATTAQUE)
        printf("%d a l'attaque adverse, min. %d", bn.modificateur, bn.minimum);

    if (bn.type == AUGMENTER_PUISSANCE)
        printf("+%d a la puissance", bn.modificateur);

    if (bn.type == DIMINUER_PUISSANCE)
        printf("%d a la puissance adverse, min. %d", bn.modificateur, bn.minimum);

    if (bn.type == AUGMENTER_DEGATS)
        printf("+%d aux degats", bn.modificateur);

    if (bn.type == DIMINUER_DEGATS)
        printf("%d aux degats adverses, min. %d", bn.modificateur, bn.minimum);

    if (bn.type == STOP_BONUS)
        printf("Stop bonus");

    if (bn.type == STOP_POUVOIR)
        printf("Stop pouvoir");

    if (bn.type == PROTECTION_POUVOIR)
        printf("Protection pouvoir");

    if (bn.type == PERTE_VIE)
        printf("%d vie, min. %d", bn.modificateur, bn.minimum);

    if (bn.type == GAIN_VIE)
        printf("+%d vie", bn.modificateur);

    if (bn.type == POISON)
        printf("Poison %d, min. %d", bn.modificateur, bn.minimum);

    if (bn.type == NEANT)
        printf("Aucun bonus");
}

void whatAboutPersos(Carte cards[4])
{
    int i = 0;
    for (i = 0 ; i < 4 ; i++)
        {
            printf("\n\n%s : %d etoiles des %s.\nPuissance : %d\tDegats : %d\nPouvoir : ", cards[i].nom, cards[i].nombreDEtoiles, cards[i].clan, cards[i].puissance, cards[i].degatsDeBase);
            whatAboutPvr(cards[i].pouvoir);

            printf("\nBonus : ");
            whatAboutbn(cards[i].bonus);
        }
    printf("\n\n\n\n\n");
}



