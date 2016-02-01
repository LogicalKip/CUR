#include <string>
#include <sstream>
#include <iostream>
#include "../include/main.hpp"
#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"
#include "../include/fonctionsAnnexes.hpp"


using std::cerr;
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::stringstream;

#if defined (WIN32)
    #include <windows.h>
    void effacerEcran() { 
        system("CLS"); 
    }
#elif defined (__linux)
    void effacerEcran(){ 
        system("clear"); 
    }
#endif

void erreur(string message)
{
    cerr << endl << endl << "/!\\Something that should never happen in the program actually happened (:O) : " << message << endl 
    << "Press Enter to quit" << endl;
    getchar();
    exit(0);
}

void changerMajuscules(string& nom)
{
    for (unsigned int i = 0 ; i < nom.length() ; i++)
        nom[i] = equivalentMinuscule(nom[i]);

    for (unsigned int i = 0 ; i < nom.length() ; i++)
    {
        if (nom[i] == '_' && nom[i+1] != '\0')// Même si, a priori, aucun nom de finira par \0...
            nom[i+1] = equivalentMajuscule(nom[i+1]);
    }

    nom[0] = equivalentMajuscule(nom[0]);
}

void whatHappenedRound1(int *pillz, int *pillzAdverses, int *pointsDeVie, int *pointsDeVieAdverses)
{//mMhmmhm... peut-etre changer l'ordre des questions en fonction de qui commence, pour paraître plus logique...
    int carteEnvoyeeEnnemie = 0, pillzUtilisesParLEnnemi = 0, carteEnvoyeeAlliee = 0, pillzUtilisesAllies = 0;
    bool ilUtiliseFury = false, jUtiliseFury = false, nomInexistant = false;
    string nomEnnemi;
    string nomAllie;

    do
    {//qui l'ennemi a-t-il envoye?
        cout << "\nQuelle carte a envoye ton adversaire au premier round ? ";
        cin >> nomEnnemi;

        changerMajuscules(nomEnnemi);

        for (int i = 0 ; i < 4 ; i++)
        {
            if (carteEnnemie[i].nom == nomEnnemi)
            {
                carteEnvoyeeEnnemie = i;
                nomInexistant = false;
                break;
            }

            else
                nomInexistant = true;
        }

        if (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || nomInexistant)
            cout << "\nEuh.... Ou pas?\n";
    }
    while (carteEnvoyeeEnnemie < 0 || carteEnvoyeeEnnemie > 3 || nomInexistant);

    carteEnnemie[carteEnvoyeeEnnemie].utiliseeACoupSur = 1;

    ilAvaitMisCombienDePillz(*pillzAdverses, &pillzUtilisesParLEnnemi, &ilUtiliseFury);

    do
    {//qui l'utilisateur a-t-il envoye?
        cout << "\n\nEt toi, tu as envoye quelle carte au premier round ? ";
        cin >> nomAllie;

        changerMajuscules(nomAllie);

        for (int i = 0 ; i < 4 ; i++)
        {
            if (carteAlliee[i].nom == nomAllie)
            {
                carteEnvoyeeAlliee = i;
                nomInexistant = false;
                break;
            }

            else
                nomInexistant = true;
        }

        if (carteEnvoyeeAlliee < 0 || carteEnvoyeeAlliee > 3 || nomInexistant)
            cout << "\nNon, franchement...\n";
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
        erreur("fonctionsAnnexes/whatHappenedRound1 : combat is not of any expected value");
}

void testFinDeJeu(int pointsDeVieAdverses, int pointsDeVie)
{
    if (pointsDeVie < 1)
        {
            cout << endl << "Sorry... I did what I could, but fate has failed you...\n\n\n... unless it's the programmer ^^." << endl << endl << "Better luck next time !" << endl;
        }

    if (pointsDeVieAdverses < 1)
        {
            #if defined (WIN32)//small gift for windows users who won ;)
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
            cout << endl << "Yeah ! Who's the best ? Alright, see you next time !" << endl;
            quit();
        }
}

void remiseAZero()
{
    for (int i = 0 ; i < 4 ; i++)
        {// aucune carte n'est utilisee !! Ou alors ca se saura grâce a utiliseeACoupSur. Ici ce sont les variables utilisees dans les boucles (qui servent a eviter de calculer une carte dont on considere qu'elle a ete utilisee lors des rounds precedents)
            carteAlliee[i].supposeeUtilisee = false;
            carteEnnemie[i].supposeeUtilisee = false;
        }

    for (int j = 0 ; j < 4 ; j++)//mise a 0 des victoires
        {
            for (int i = 0 ; i <= 12 ; i++)//TODO condition de fin de boucle a changer?
                {
                    carteAlliee[j].victoiresAvecXpillzEntreCrochets[i] = 0;
                    carteAlliee[j].defaitesAvecXpillz[i] = 0;
                    carteAlliee[j].egalitesAvecXPillz[i] = 0;
                    carteAlliee[j].guessedScore[i] = 0;
                }
        }
    ///Inutile de reset les ennemis, car on ne se sert pas de leurs nombresDeVictoires
    for (int j = 0 ; j < 4 ; j++)//mise a 0 des victoires
        {
            for (int i = 0 ; i <= 9 ; i++)//TODO condition de fin de boucle a changer?
                {
                    carteAlliee[j].victoiresAvecXpillzEtFury[i] = 0;
                    carteAlliee[j].defaitesAvecXpillzEtFury[i] = 0;
                    carteAlliee[j].egalitesAvecXPillzEtFury[i] = 0;
                    carteAlliee[j].guessedScoreFury[i] = 0;
                }
        }

    persoTest = 0;
    pillzTest = 0;
    furyUtilisee = false;
}

string whatAboutPillzHP(int pillz, int pillzAdverses, int pointsDeVie, int pointsDeVieAdverses)
{
	stringstream res;
	
	res << "\n\nYou :\n\tHealth : " << pointsDeVie << "\n\tPillz : " << pillz << "\n\nThe opponent :\n\tHealth : " << pointsDeVieAdverses << "\n\tPillz : " << pillzAdverses << "\n\n";
	
	return res.str();
}

void affichageDesVictoires()
{
    char afficher = 0;

    cout << "\nDisplay computed results ? (" << YES_CHAR << "/" << NO_CHAR << ") : ";

    while(afficher != YES_CHAR && afficher != NO_CHAR)
       cin >> afficher;

    if (afficher == YES_CHAR)
    {
        cout << "\n\n\n";
        for (int j = 0 ; j < 4 ; j++)
            {//affichage des victoires histoire de verifier
                if (! carteAlliee[j].utiliseeACoupSur)
                    {
                        cout << "Victoires de " << carteAlliee[j].nom << " :" << endl;

                        for (int i = 0 ; i <= 12 ; i++)
                        {
                            cout << "\t";
                            if (i < 10)
                                cout << " ";

                            printf("%d pillz : V=%d E=%d D=%d", i, carteAlliee[j].victoiresAvecXpillzEntreCrochets[i], carteAlliee[j].egalitesAvecXPillz[i], carteAlliee[j].defaitesAvecXpillz[i]);

                            if (i <= 9)
                                printf("\n\t %d pillz et fury : V=%d E=%d D=%d", i, carteAlliee[j].victoiresAvecXpillzEtFury[i], carteAlliee[j].egalitesAvecXPillzEtFury[i], carteAlliee[j].defaitesAvecXpillzEtFury[i]);

                            cout << "\n\n";
                        }
                        cout << "\n\n\n";
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
                cout << "Who starts ? Type 1 if the enemy starts, 0 if you do" << endl;
                cin >> repJoueur;

                if (repJoueur != 0 && repJoueur != 1)
                    cout << endl << "Now, that's hilarious..." << endl;
            }while (repJoueur != 0 && repJoueur != 1);

            ennemiCommence = (repJoueur == 1);
        }
}

void whatAboutAbility(CardAbility pvr)
{
    if (pvr.condition == SUPPORT)
        cout << "(support) ";

    if (pvr.condition == COURAGE)
        cout << "Courage : ";

    if (pvr.condition == CONFIANCE)
        cout << "Confidence : ";

    if (pvr.condition == REVANCHE)
        cout << "Revenge : ";

    if (pvr.condition == EN_CAS_DE_STOP)
        cout << "Stop : ";

    if (pvr.condition == DEFAITE)
        cout << "Defeat : ";

    if (pvr.type == AUGMENTER_ATTAQUE)
        printf("+%d to attack", pvr.modificateur);

    if (pvr.type == DIMINUER_ATTAQUE)
        printf("%d to opponent attack, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == AUGMENTER_PUISSANCE)
        printf("+%d to power", pvr.modificateur);

    if (pvr.type == DIMINUER_PUISSANCE)
        printf("%d to opponent power, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == AUGMENTER_DEGATS)
        printf("+%d to damage", pvr.modificateur);

    if (pvr.type == COPIER_PUISSANCE)
        cout << "Power = opponent power";

    if (pvr.type == COPIER_DEGATS)
        cout << "Damage = opponent damage";

    if (pvr.type == DIMINUER_DEGATS)
        printf("%d to opponent damage, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == PROTECTION_BONUS)
        cout << "Protect bonus";

    if (pvr.type == STOP_BONUS)
        cout << "Stop bonus";

    if (pvr.type == STOP_POUVOIR)
        cout << "Stop power";

    if (pvr.type == PERTE_PILLZ)
        printf("%d pillz, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == PERTE_VIE)
        printf("%d health, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == GAIN_PILLZ)
        printf("+%d pillz", pvr.modificateur);

    if (pvr.type == GAIN_VIE)
        printf("+%d health", pvr.modificateur);

    if (pvr.type == POISON)
        printf("Poison %d, min. %d", pvr.modificateur, pvr.minimum);

    if (pvr.type == NOTHING)
        cout << "None";
}

void whatAboutPersos(Carte cards[4])
{
    for (int i = 0 ; i < 4 ; i++)
        {
            cout << endl << endl << cards[i].nom << " : " << cards[i].nombreDEtoiles << " star from " << cards[i].clan << "." << endl << "Power : " << cards[i].puissance << "\tDamage : " << cards[i].degatsDeBase << endl << "Ability : ";
            whatAboutAbility(cards[i].pouvoir);

            cout << endl << "Bonus : ";
            whatAboutAbility(cards[i].bonus);
        }
    cout << "\n\n\n\n\n";
}



