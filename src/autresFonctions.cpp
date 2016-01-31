#include <iostream>


#include "../include/main.hpp"
#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"

using std::cin;
using std::cout;
using std::string;
using std::endl;


int carteEnvoyeeParLEnnemiEnCourage(int *carteQuIlEnvoie, int round)
{
    string nomDeLaCarte;
    int carteEnvoyeeEnnemie = 23;
    bool nomInexistant = false;

    do
    {
        if (round == 1)
            cout << "\n\n\nQuelle carte envoie l'ennemi pour le 1er round ? --> ";
        else
            cout << "\n\n\nQuelle carte envoie l'ennemi pour le " << round << "eme round ? --> ";

        cin >> nomDeLaCarte;

        changerMajuscules(nomDeLaCarte);

        nomInexistant = true;
        for (int i = 0 ; i < 4 ; i++)
        {
            if (carteEnnemie[i].nom == nomDeLaCarte)
            {
                carteEnvoyeeEnnemie = i;
                nomInexistant = false;
                break;
            }
        }

        if (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || nomInexistant)
            cout << "\nAh je ne pense pas, non !\n";

        else if (carteEnnemie[carteEnvoyeeEnnemie].utiliseeACoupSur && !nomInexistant)
            cout << "\nCette carte a deja ete utilisee !\n";
    }
    while (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || carteEnnemie[carteEnvoyeeEnnemie].utiliseeACoupSur);

    *carteQuIlEnvoie = carteEnvoyeeEnnemie;

    return carteEnvoyeeEnnemie;
}

void reponseEnnemie(int *pillzAdverses, int *pillzQuIlUtilise, int *carteQuIlEnvoie, bool *ennemiUtiliseFury, int round)
{
    int carteEnnemieEnReponse = 0;
    bool nomInexistant = false, dejaUtilisee = false;
    string nomDeLaCarte;

    do
    {//test de veracite
        dejaUtilisee = false;
        nomInexistant = false;
        if (round == 1)
            cout << "Alors, finalement qui a-t-il envoye lors de ce 1er round ? : ";

        else
            cout << "Alors, finalement qui a-t-il envoye lors de ce " << round << "eme round ? : ";//pour le 4eme round il a pas le choix...

        cin >> nomDeLaCarte;

        changerMajuscules(nomDeLaCarte);

        for (int i = 0 ; i < 4 ; i++)
        {
            if (carteEnnemie[i].nom == nomDeLaCarte)
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
            cout << "\nEuuh.. Ou pas ?\n";

        else if (dejaUtilisee)
            cout << "\nCette carte a deja ete utilisee !\n";
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
            cout << "\nCombien de pillz sur la carte ? ";
            cin >> pillzMis;

            if (pillzMis > pillzAdverses || pillzMis < 0)
                cout << "C'est impossible...\n";
        }
        while (pillzMis > pillzAdverses || pillzMis < 0);
    }

    else
        pillzMis = 0;

    if (pillzAdverses - pillzMis >= 3)
    {
        cout << "\nAvec la fury ? (o/n) : ";

        while(furyOuPas != 'o' && furyOuPas != 'n')
        {
            cin >> furyOuPas;
            furyOuPas = equivalentMinuscule(furyOuPas);
        }

        *ennemiUtiliseFury = (furyOuPas == 'o');
    }

    else
        *ennemiUtiliseFury = false;

    *pillzQuIlUtilise = pillzMis;
}

/**
 * permet de savoir si carteATester est un meilleur choix (avec un nombre de pillz donné) que le précédent
 * Pour l'instant : on prend la carte avec le plus gros pourcentage de (victoires+egalités), et en cas d'égalité (pas le même sens, attention), celle qui a un plus gros pourcentage de victoires
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
    double pourcentageVEcarteATester = (double) (vicTest+egaTest) / (vicTest+egaTest+defTest);//multiplier par 1.0 avant la division si la conversion ne se fait pas comme prévu
    double pourcentageVEMCC = (double) (vicMCC+egaMCC) / (vicMCC+egaMCC+defMCC);

    if (pourcentageVEcarteATester > 0)
    {
        if (pourcentageVEcarteATester > pourcentageVEMCC)
            estMeilleur = true;
        else if (pourcentageVEcarteATester == pourcentageVEMCC)
        {
            // permet de savoir si il y a un plus gros pourcentage de victoires parmi (victoires+egalités)
            double vicSurVETest = (double) vicTest / (vicTest+egaTest), vicSurVEMCC = (double) vicMCC / (vicMCC+egaMCC);
            estMeilleur = vicSurVETest > vicSurVEMCC;
        }
    }

    return estMeilleur;
}


void faisCa(int *pillz, int *pillzQueJUtilise, int *carteQueJEnvoie, bool *ilFautUtiliserLaFury)
{
    int pillzAUtiliser = 0;
    bool victoireACoupSur = false, defaiteACoupSur = true;
    Carte onlyDefaite;//carte bidon telle qu'une comparaison avec une vraie carte déterminera forcément la vraie carte comme un meilleur choix (sauf si la vraie est elle aussi à 0%...)
    for (int i = 0 ; i <= 12 ; i++)
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

    for (int i = 0 ; i < 4 ; i++)
        {//calcul des persos/pillzs a utiliser avec la fury
            if (! carteAlliee[i].utiliseeACoupSur)
            {
                for (int j = 0 ; j <= *pillz - 3 ; j++)
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

        for (int i = 0 ; i < 4 ; i++)
    {//calcul des persos/pillzs a utiliser
        if (! carteAlliee[i].utiliseeACoupSur)
        {
            for (int j = 0 ; j <= *pillz ; j++)
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
                        *ilFautUtiliserLaFury = false;//ca va se repeter beacoup de fois inutiles, non? mais tant pis c'est plus rapide d'écrire que de vérifier
                    }
                }
            }
        }
    }


    if (defaiteACoupSur)
    {
        cout << "\nJe suis desole... Tu n'as pas la moindre chance de t'en sortir...\n\n\n\nSauf si l'autre a une coupure de courant !\nAppuie sur Entree pour quitter\n";
        getchar();
        exit(0);
    }
    else
    {
        cout << endl << "Il faut utiliser " << carteAUtiliser->nom;

        if (*pillz > 0)
            cout << " avec " << pillzAUtiliser << " pillz";

        if (*ilFautUtiliserLaFury)
            cout << " et la fury";

        if (victoireACoupSur)
            cout << " (VICTOIRE A COUP SUR)";

        cout << " (" << carteAUtiliser->victoiresAvecXpillzEntreCrochets[pillzAUtiliser] << " victoires eventuelles)\n\n";

        cout << "Je suppose que tu as suivi mon conseil.\nJe considere donc cette carte ";
        if (*pillz > 0)
            cout << "et ces pillz comme utilises.\n";
        else
            cout << "comme utilisee.\n";

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

    //FIXME possible de remplacer la condition par == vainqueur || egaliteDAttaque() (et else : modif en tant que perdant ?) le résultat est-il exactement le même ?
    //qui a gagné
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
    traiterRound(pillz, pillzAdverses, pointsDeVie, pointsDeVieAdverses, round, ennemiCommence, X_PREMIERS_ROUNDS_A_TESTER);
#endif
}

void traiterRound(int* pillz, int* pillzAdverses, int* pointsDeVie, int* pointsDeVieAdverses, int* round, bool courageEnnemi, int dernierRoundACalculer = 4)
{
    int pillzQueJUtilise = 0, pillzQuIlUtilise = 0, carteQueJEnvoie = 0, carteQuIlEnvoie = 0, carteCourageEnnemie = -1;
    bool ilFautUtiliserLaFury = false, ennemiUtiliseFury = false;

    (*round)++;
    cout << "\n\nLe " << *round << "eme round va commencer\n";

    cout << whatAboutPillzHP(*pillz, *pillzAdverses, *pointsDeVie, *pointsDeVieAdverses);

    remiseAZero();

    if (courageEnnemi)
        carteCourageEnnemie = carteEnvoyeeParLEnnemiEnCourage(&carteQuIlEnvoie, *round);

    calculerRound(*round, *pointsDeVie, *pointsDeVieAdverses, *pillz, *pillzAdverses, true, carteCourageEnnemie, dernierRoundACalculer);
    cout << "Calcul termine.\n";


	if (dernierRoundACalculer == 4) { // Decided to brute force all 4 rounds
		affichageDesVictoires();
		faisCa(pillz, &pillzQueJUtilise, &carteQueJEnvoie, &ilFautUtiliserLaFury);
	} else {
		int max = carteAlliee[0].guessedScore[0];//FIXME et si déjà envoyée, lors d'un round >1 ?
		int c = 0, p = 0;
		bool f = false;
		for (int i = 0 ; i < 4 ; i++) {
			for (int j = 0 ; j < *pillz ; j++) {
				if (!carteAlliee[i].utiliseeACoupSur && carteAlliee[i].guessedScore[j] > max) {
					max = carteAlliee[i].guessedScore[j];
					c = i;
					p = j;
					f = false;
				}	
				if (!carteAlliee[i].utiliseeACoupSur && carteAlliee[i].guessedScoreFury[j] > max) {
					max = carteAlliee[i].guessedScore[j];
					c = i;
					p = j;
					f = true;
				}		
			}
		}
		
		cout << "Il faut probablement envoyer " << carteAlliee[c].nom << " avec " << p << " pillz";
		if (f) {
			cout << " et la fury (heuristique = " << carteAlliee[c].guessedScoreFury[p] << ")";//FIXME m'a déjà proposé 10 pillz et la fury
		} else {	
			cout << " (heuristique = " << carteAlliee[c].guessedScore[p] << ")";
		}
		
		carteQueJEnvoie = c;
		pillzQueJUtilise = p;
		ilFautUtiliserLaFury = f;
	}

    if (courageEnnemi)
    {
        if (*pillzAdverses > 0)
            cout << "\nCa y est, c'est fait ?\nDis-moi combien de pillz avait mis l'ennemi sur sa carte des que tu peux.\n";

        ilAvaitMisCombienDePillz(*pillzAdverses, &pillzQuIlUtilise, &ennemiUtiliseFury);
    }
    else
    {
        cout << "\nCa y est, c'est fait ? Dis-moi ce qu'a fait l'ennemi en reponse des que tu peux.\n";
        reponseEnnemie(pillzAdverses, &pillzQuIlUtilise, &carteQuIlEnvoie, &ennemiUtiliseFury, *round);
    }

    majPvPillz(pointsDeVie, pointsDeVieAdverses, pillz, pillzAdverses, carteQuIlEnvoie, carteQueJEnvoie, pillzQuIlUtilise, pillzQueJUtilise, *round, ilFautUtiliserLaFury, ennemiUtiliseFury);

    testFinDeJeu(*pointsDeVieAdverses, *pointsDeVie);
}

void processGame(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses) 
{
    int round = 0;

    gererPremierRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round);

    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, !ennemiCommence /*(courageEnnemi)*/);//je commence les rounds 2 et 4
    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round,  ennemiCommence);
    traiterRound(&pillz, &pillzAdverses, &pointsDeVie, &pointsDeVieAdverses, &round, !ennemiCommence);
}

