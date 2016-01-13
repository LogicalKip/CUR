#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/main.hpp"
#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"


int carteEnvoyeeParLEnnemiEnCourage(int *carteQuIlEnvoie, int round)
{
    char nomDeLaCarte[15] = "UNDEFINED";
    int carteEnvoyeeEnnemie = 23, i = 0;
    bool nomInexistant = false;

    do
    {
        if (round == 1)
            printf("\n\n\nQuelle carte envoie l'ennemi pour le 1er round ? --> ");
        else
            printf("\n\n\nQuelle carte envoie l'ennemi pour le %deme round ? --> ", round);

        scanf("%s", nomDeLaCarte);

        changerMajuscules(nomDeLaCarte);

        nomInexistant = true;
        for (i = 0 ; i < 4 ; i++)
        {
            if (strcmp(carteEnnemie[i].nom, nomDeLaCarte) == 0)
                {
                    carteEnvoyeeEnnemie = i;
                    nomInexistant = false;
                    break;
                }
        }

        if (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || nomInexistant)
            printf("\nAh je ne pense pas, non !\n");

        else if (carteEnnemie[carteEnvoyeeEnnemie].utiliseeACoupSur && !nomInexistant)
            printf("\nCette carte a deja ete utilisee !\n");
    }
    while (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || carteEnnemie[carteEnvoyeeEnnemie].utiliseeACoupSur);

    *carteQuIlEnvoie = carteEnvoyeeEnnemie;

    return carteEnvoyeeEnnemie;
}

void reponseEnnemie(int *pillzAdverses, int *pillzQuIlUtilise, int *carteQuIlEnvoie, bool *ennemiUtiliseFury, int round)
{
    int carteEnnemieEnReponse = 0, i = 0;
    bool nomInexistant = false, dejaUtilisee = false;
    char nomDeLaCarte[15] = "rien";

    do
    {//test de veracite
        dejaUtilisee = false;
        nomInexistant = false;
        if (round == 1)
            printf("Alors, finalement qui a-t-il envoye lors de ce 1er round ? : ");

        else
            printf("Alors, finalement qui a-t-il envoye lors de ce %deme round ? : ", round);//pour le 4eme round il a pas le choix...

        scanf("%s", nomDeLaCarte);

        changerMajuscules(nomDeLaCarte);

        for (i = 0 ; i < 4 ; i++)
        {
            if (strcmp(carteEnnemie[i].nom, nomDeLaCarte) == 0)
                {
                    if (carteEnnemie[i].utiliseeACoupSur)
                        dejaUtilisee = true;
                    else
                        {
                            carteEnnemieEnReponse = i;
                            nomInexistant = false;
                            dejaUtilisee = false;
                            break;
                        }
                }
            else
                nomInexistant = true;
        }

        if (carteEnnemieEnReponse < 0 || carteEnnemieEnReponse > 3 || (nomInexistant && !dejaUtilisee))
            printf("\nEuuh.. Ou pas ?\n");

        else if (dejaUtilisee)
            printf("\nCette carte a deja ete utilisee !\n");
    }
    while (carteEnnemie[carteEnnemieEnReponse].utiliseeACoupSur || carteEnnemieEnReponse < 0 || carteEnnemieEnReponse > 3 || dejaUtilisee || nomInexistant);

    carteEnnemie[carteEnnemieEnReponse].utiliseeACoupSur = true;

    *carteQuIlEnvoie = carteEnnemieEnReponse;

    ilAvaitMisCombienDePillz(*pillzAdverses, pillzQuIlUtilise, ennemiUtiliseFury);
}

void ilAvaitMisCombienDePillz(int pillzAdverses, int *pillzQuIlUtilise, bool *ennemiUtiliseFury)
{
    int pillzMis = 0;
    char furyOuPas = 'a';

    if (pillzAdverses > 0)
    {
        do
        {
            printf("\nCombien de pillz sur la carte ? ");
            scanf("%d", &pillzMis);

            if (pillzMis > pillzAdverses || pillzMis < 0)
                printf("C'est impossible...\n");
        }
        while (pillzMis > pillzAdverses || pillzMis < 0);
    }

    else
        pillzMis = 0;

    if (pillzAdverses - pillzMis >= 3)
        {
            printf("\nAvec la fury ? (o/n) : ");

            while(furyOuPas != 'o' && furyOuPas != 'n')
            {
                scanf("%c", &furyOuPas);
                furyOuPas = equivalentMinuscule(furyOuPas);
            }

            *ennemiUtiliseFury = (furyOuPas == 'o');
        }

    else
        *ennemiUtiliseFury = false;

    *pillzQuIlUtilise = pillzMis;
}

/**
 * permet de savoir si carteATester est un meilleur choix (avec un nombre de pillz donn�) que le pr�c�dent
 * Pour l'instant : on prend la carte avec le plus gros pourcentage de (victoires+egalit�s), et en cas d'�galit� (pas le m�me sens, attention), celle qui a un plus gros pourcentage de victoires
 */
bool estMeilleur(Carte carteATester, int pillzATester, bool considererFuryPourTest/* si on traite que la fury (ou pas du tout) */, Carte meilleureCarteCourante, int pillzPourMeilleurChoixCourant, bool furyUtiliseePourMeilleurChoixCourant)
{
    // On raccourcit les noms de variable
    int vicTest = carteATester.victoiresAvecXpillzEntreCrochets[pillzATester], defTest = carteATester.defaitesAvecXpillz[pillzATester], egaTest = carteATester.egalitesAvecXPillz[pillzATester];
    int vicMCC = meilleureCarteCourante.victoiresAvecXpillzEntreCrochets[pillzPourMeilleurChoixCourant], defMCC = meilleureCarteCourante.defaitesAvecXpillz[pillzPourMeilleurChoixCourant], egaMCC = meilleureCarteCourante.egalitesAvecXPillz[pillzPourMeilleurChoixCourant];
    //MCC = Meilleure Carte Courante
    bool estMeilleur = false;
    if (considererFuryPourTest) {
        vicTest = carteATester.victoiresAvecXpillzEtFury[pillzATester];
        defTest = carteATester.defaitesAvecXpillzEtFury[pillzATester];
        egaTest = carteATester.egalitesAvecXPillzEtFury[pillzATester];
    }
    if (furyUtiliseePourMeilleurChoixCourant) {
        vicMCC = meilleureCarteCourante.victoiresAvecXpillzEtFury[pillzPourMeilleurChoixCourant];
        defMCC = meilleureCarteCourante.defaitesAvecXpillzEtFury[pillzPourMeilleurChoixCourant];
        egaMCC = meilleureCarteCourante.egalitesAvecXPillzEtFury[pillzPourMeilleurChoixCourant];
    }

    // (Victoires+Egalites)/total
    double pourcentageVEcarteATester = (double) (vicTest+egaTest) / (vicTest+egaTest+defTest);//multiplier par 1.0 avant la division si la conversion ne se fait pas comme pr�vu
    double pourcentageVEMCC = (double) (vicMCC+egaMCC) / (vicMCC+egaMCC+defMCC);

    if (pourcentageVEcarteATester > 0)
    {
        if (pourcentageVEcarteATester > pourcentageVEMCC)
            estMeilleur = true;
        else if (pourcentageVEcarteATester == pourcentageVEMCC)
        {
            // permet de savoir si il y a un plus gros pourcentage de victoires parmi (victoires+egalit�s)
            double vicSurVETest = (double) vicTest / (vicTest+egaTest), vicSurVEMCC = (double) vicMCC / (vicMCC+egaMCC);
            estMeilleur = vicSurVETest > vicSurVEMCC;
        }
    }

    return estMeilleur;
}


void faisCa(int *pillz, int *pillzQueJUtilise, int *carteQueJEnvoie, bool *ilFautUtiliserLaFury)
{
    int i = 0, j = 0, pillzAUtiliser = 0;
    bool victoireACoupSur = false, defaiteACoupSur = true;
    Carte onlyDefaite;//carte bidon telle qu'une comparaison avec une vraie carte d�terminera forc�ment la vraie carte comme un meilleur choix (sauf si la vraie est elle aussi � 0%...)
    for (i = 0 ; i <= 12 ; i++)
    {
        if (i <= 9)
        {
             onlyDefaite.defaitesAvecXpillzEtFury[i] = 10;
             onlyDefaite.egalitesAvecXPillzEtFury[i] = 0;
             onlyDefaite.victoiresAvecXpillzEtFury[i] = 0;
        }
        onlyDefaite.defaitesAvecXpillz[i] = 10;
        onlyDefaite.victoiresAvecXpillzEntreCrochets[i] = 0;///ou -1 ?
        onlyDefaite.egalitesAvecXPillz[i] = 0;
    }


    Carte *carteAUtiliser = &onlyDefaite;

    *ilFautUtiliserLaFury = false;

    for (i = 0 ; i < 4 ; i++)
        {//calcul des persos/pillzs a utiliser avec la fury
            if (! carteAlliee[i].utiliseeACoupSur)
                {
                    for (j = 0 ; j <= *pillz - 3 ; j++)
                        {
                            if (! victoireACoupSur)
                                {
                                    if (carteAlliee[i].defaitesAvecXpillzEtFury[j] == 0 && carteAlliee[i].egalitesAvecXPillzEtFury[j] == 0 && carteAlliee[i].victoiresAvecXpillzEtFury[j] > 0)
                                        {
                                            carteAUtiliser = &carteAlliee[i];
                                            pillzAUtiliser = j;
                                            *ilFautUtiliserLaFury = true;
                                            victoireACoupSur = true;
                                            defaiteACoupSur = false;
                                        }

                                    else if (estMeilleur(carteAlliee[i], j, true, *carteAUtiliser, pillzAUtiliser, *ilFautUtiliserLaFury))
                                        {
                                            carteAUtiliser = &carteAlliee[i];
                                            pillzAUtiliser = j;
                                            defaiteACoupSur = false;
                                            *ilFautUtiliserLaFury = true;
                                        }
                                }
                        }
                }
        }

    for (i = 0 ; i < 4 ; i++)
        {//calcul des persos/pillzs a utiliser
            if (! carteAlliee[i].utiliseeACoupSur)
                {
                    for (j = 0 ; j <= *pillz ; j++)
                        {
                            if (! victoireACoupSur)
                                {
                                    if (carteAlliee[i].defaitesAvecXpillz[j] == 0 && carteAlliee[i].egalitesAvecXPillz[j] == 0 && carteAlliee[i].victoiresAvecXpillzEntreCrochets[j] > 0)
                                        {
                                            carteAUtiliser = &carteAlliee[i];
                                            pillzAUtiliser = j;
                                            *ilFautUtiliserLaFury = false;
                                            victoireACoupSur = true;
                                            defaiteACoupSur = false;
                                        }

                                    else if (estMeilleur(carteAlliee[i], j, false, *carteAUtiliser, pillzAUtiliser, *ilFautUtiliserLaFury))
                                        {
                                            carteAUtiliser = &carteAlliee[i];
                                            pillzAUtiliser = j;
                                            defaiteACoupSur = false;
                                            *ilFautUtiliserLaFury = false;//ca va se repeter beacoup de fois inutiles, non? mais tant pis c'est plus rapide d'�crire que de v�rifier
                                        }
                                }
                        }
                }
        }


    if (defaiteACoupSur)
        {
            printf("\nJe suis desole... Tu n'as pas la moindre chance de t'en sortir...\n\n\n\nSauf si l'autre a une coupure de courant !\nAppuie sur Entree pour quitter\n");
            getchar();
            exit(0);
        }
    else
        {
            printf("\nIl faut utiliser %s", carteAUtiliser->nom);

            if (*pillz > 0)
                printf(" avec %d pillz", pillzAUtiliser);

            if (*ilFautUtiliserLaFury)
                printf(" et la fury");

            if (victoireACoupSur)
                printf(" (VICTOIRE A COUP SUR)");

            printf(" (%d victoires eventuelles)\n\n", carteAUtiliser->victoiresAvecXpillzEntreCrochets[pillzAUtiliser]);

            printf("Je suppose que tu as suivi mon conseil.\nJe considere donc cette carte ");
            if (*pillz > 0)
                printf("et ces pillz comme utilises.\n");
            else
                printf("comme utilisee.\n");

            int indicecarteAUtiliser = 0;
            while (&carteAlliee[indicecarteAUtiliser] != carteAUtiliser)
                indicecarteAUtiliser++;
            *carteQueJEnvoie = indicecarteAUtiliser;
            *pillzQueJUtilise = pillzAUtiliser;
        }
}

void majPvPillz(int *pointsDeVie, int *pointsDeVieAdverses, int *pillz, int *pillzAdverses, int carteEnvoyeeParEnnemi, int carteQueJEnvoie, int pillzUtilisesParEnnemi, int pillzQueJUtilise, int round, int ilFautUtiliserLaFury, int furyEnnemie)//la MAJ des pillz doit se faire ici
{
    carteEnnemie[carteEnvoyeeParEnnemi].utiliseeACoupSur = 1;
    carteAlliee[carteQueJEnvoie].utiliseeACoupSur = 1;

    *pillz -= pillzQueJUtilise;
    *pillzAdverses -= pillzUtilisesParEnnemi;

    if (ilFautUtiliserLaFury)
        *pillz -= 3;
    if (furyEnnemie)
        *pillzAdverses -= 3;

    //FIXME possible de remplacer la condition par == vainqueur || egaliteDAttaque() (et else : modif en tant que perdant ?) le r�sultat est-il exactement le m�me ?
    //qui a gagn�
    if (carteAlliee[carteQueJEnvoie].combatAvecXPillzContreYAvecZpillz[pillzQueJUtilise][carteEnvoyeeParEnnemi][pillzUtilisesParEnnemi] == VAINQUEUR)
        {// si je le bats
            modifPerteGainVie(carteQueJEnvoie, pointsDeVie, carteAlliee[carteQueJEnvoie], carteEnvoyeeParEnnemi, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeParEnnemi], ilFautUtiliserLaFury);
            modifPerteGainPillz(carteQueJEnvoie, pillz, carteAlliee[carteQueJEnvoie], carteEnvoyeeParEnnemi, pillzAdverses, carteEnnemie[carteEnvoyeeParEnnemi]);
        }

    else if (carteAlliee[carteQueJEnvoie].combatAvecXPillzContreYAvecZpillz[pillzQueJUtilise][carteEnvoyeeParEnnemi][pillzUtilisesParEnnemi] == PERDANT)
        {// si il me bat
            modifPerteGainVie(carteEnvoyeeParEnnemi, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeParEnnemi], carteQueJEnvoie, pointsDeVie, carteAlliee[carteQueJEnvoie], furyEnnemie);
            modifPerteGainPillz(carteEnvoyeeParEnnemi, pillzAdverses, carteEnnemie[carteEnvoyeeParEnnemi], carteQueJEnvoie, pillz, carteAlliee[carteQueJEnvoie]);
        }

    else if (carteAlliee[carteQueJEnvoie].combatAvecXPillzContreYAvecZpillz[pillzQueJUtilise][carteEnvoyeeParEnnemi][pillzUtilisesParEnnemi] == EGALITE)
        {//egalite
            if (egaliteDAttaque(round))//return true : j'ai gagne
                {// si je le bats
                    modifPerteGainVie(carteQueJEnvoie, pointsDeVie, carteAlliee[carteQueJEnvoie], carteEnvoyeeParEnnemi, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeParEnnemi], ilFautUtiliserLaFury);
                    modifPerteGainPillz(carteQueJEnvoie, pillz, carteAlliee[carteQueJEnvoie], carteEnvoyeeParEnnemi, pillzAdverses, carteEnnemie[carteEnvoyeeParEnnemi]);
                }

            else if (carteAlliee[carteQueJEnvoie].combatAvecXPillzContreYAvecZpillz[pillzQueJUtilise][carteEnvoyeeParEnnemi][pillzUtilisesParEnnemi] == PERDANT)
                {// si il me bat
                    modifPerteGainVie(carteEnvoyeeParEnnemi, pointsDeVieAdverses, carteEnnemie[carteEnvoyeeParEnnemi], carteQueJEnvoie, pointsDeVie, carteAlliee[carteQueJEnvoie], furyEnnemie);
                    modifPerteGainPillz(carteEnvoyeeParEnnemi, pillzAdverses, carteEnnemie[carteEnvoyeeParEnnemi], carteQueJEnvoie, pillz, carteAlliee[carteQueJEnvoie]);
                }
        }

    else
        erreur("autresFonctions/majPvPillz : combat ne vaut VAINQUEUR, ni PERDANT, ni EGALITE\n");
}


void gererPremierRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round)
{
#ifdef NE_PAS_TRAITER_1ER_ROUND
    whatHappenedRound1(pillz, pillzAdverses, pointsDeVie, pointsDeVieAdverses);
    (*round)++;
#else
    traiterRound(pillz, pillzAdverses, pointsDeVie, pointsDeVieAdverses, round, ennemiCommence);
#endif
}

void traiterRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round, bool courageEnnemi)
{
    int pillzQueJUtilise = 0, pillzQuIlUtilise = 0, carteQueJEnvoie = 0, carteQuIlEnvoie = 0, carteCourageEnnemie = -1;
    bool ilFautUtiliserLaFury = false, ennemiUtiliseFury = false;

    (*round)++;
    printf("\n\nLe %deme round va commencer\n", *round);

    printf(whatAboutPillzHP(*pillz, *pillzAdverses, *pointsDeVie, *pointsDeVieAdverses).c_str());

    remiseAZero();

    if (courageEnnemi)
        carteCourageEnnemie = carteEnvoyeeParLEnnemiEnCourage(&carteQuIlEnvoie, *round);

    calculerRound(*round, *pointsDeVie, *pointsDeVieAdverses, *pillz, *pillzAdverses, true, carteCourageEnnemie);
    printf("Calcul termine.\n");

    affichageDesVictoires();
    faisCa(pillz, &pillzQueJUtilise, &carteQueJEnvoie, &ilFautUtiliserLaFury);

    if (courageEnnemi)
    {
        if (*pillzAdverses > 0)
            printf("\nCa y est, c'est fait ?\nDis-moi combien de pillz avait mis l'ennemi sur sa carte des que tu peux.\n");

        ilAvaitMisCombienDePillz(*pillzAdverses, &pillzQuIlUtilise, &ennemiUtiliseFury);
    }
    else
    {
        printf("\nCa y est, c'est fait ? Dis-moi ce qu'a fait l'ennemi en reponse des que tu peux.\n");
        reponseEnnemie(pillzAdverses, &pillzQuIlUtilise, &carteQuIlEnvoie, &ennemiUtiliseFury, *round);
    }

    majPvPillz(pointsDeVie, pointsDeVieAdverses, pillz, pillzAdverses, carteQuIlEnvoie, carteQueJEnvoie, pillzQuIlUtilise, pillzQueJUtilise, *round, ilFautUtiliserLaFury, ennemiUtiliseFury);

    testFinDeJeu(*pointsDeVieAdverses, *pointsDeVie);
}

void cestLEnnemiQuiCommence(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses)
{
    int round = 0;

    gererPremierRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round);

    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, false/*(courageEnnemi)*/);//je commence les rounds 2 et 4
    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, true);
    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, false);
}

void cestMoiQuiCommence(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses)
{
    int round = 0;

    gererPremierRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round);

    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, true/*(courageEnnemi)*/);
    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, false);//je commence les rounds 1 et 3
    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, true);
}


