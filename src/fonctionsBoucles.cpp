


#include "../include/main.hpp"
#include "../include/constantes.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"

int attaque(int puissance, int pillzUtilises)
{
    return puissance * pillzUtilises + puissance;
}

bool egaliteDAttaque(int round)//return true : j'ai gagne.   return false : j'ai perdu. Ben oui c'est pas un titre explicite. Ben tant pis.
{  //egaliteDAttaque... ET D'ETOILES !
if (round == 1 || round == 3)
    return !ennemiCommence;
else 
    return ennemiCommence;
}


void modifPerteGainPillz(int numVic, int *pillzVic, Carte carteVic, int numDef, int *pillzDef, Carte carteDef)
{//TODO ne gere pas les "stop : +X pillz". Un jour, peut-être, le cas échéant...
if (carteVic.pouvoir.type == GAIN_PILLZ && carteVic.pouvoirEstActifContre[numDef] && carteVic.pouvoir.condition != DEFAITE)
    *pillzVic += carteVic.pouvoir.modificateur;

if (carteDef.pouvoir.type == GAIN_PILLZ && carteDef.pouvoir.condition == DEFAITE && carteDef.pouvoirEstActifContre[numVic])
    *pillzDef += carteDef.pouvoir.modificateur;

if (carteVic.pouvoir.type == PERTE_PILLZ && carteVic.pouvoirEstActifContre[numDef] && carteVic.pouvoir.condition != DEFAITE && *pillzDef > carteVic.pouvoir.minimum)
{
    *pillzDef += carteVic.pouvoir.modificateur;

    if (*pillzDef < carteVic.pouvoir.minimum)
        *pillzDef = carteVic.pouvoir.minimum;
}

if (carteDef.pouvoir.type == PERTE_PILLZ && carteDef.pouvoir.condition == DEFAITE && carteDef.pouvoirEstActifContre[numVic] && *pillzVic > carteDef.pouvoir.minimum)
{
    *pillzVic += carteDef.pouvoir.modificateur;

    if (*pillzVic < carteDef.pouvoir.minimum)
        *pillzVic = carteDef.pouvoir.minimum;
}
}


void modifPerteGainVie(int numVic, int *pvVic, Carte carteVic, int numDef, int *pvDef, Carte carteDef, bool furyVic) //FIXME le poison doit se faire là ?
{
//degats directs
    if (furyVic)
        *pvDef -= carteVic.degatsAvecFuryContre[numDef];
    else
        *pvDef -= carteVic.degatsContre[numDef];

//gain vie
        //pouvoir
    if (carteVic.pouvoir.type == GAIN_VIE)
    {
        if (carteVic.pouvoirEstActifContre[numDef])
        {
            if (carteVic.pouvoir.condition != EN_CAS_DE_STOP && carteVic.pouvoir.condition != DEFAITE)
                *pvVic += carteVic.pouvoir.modificateur;
        }

        else if (carteVic.pouvoir.condition == EN_CAS_DE_STOP)//le pouvoir est stoppé
        {
            if (carteVic.pouvoir.condition != DEFAITE)
                *pvVic += carteVic.pouvoir.modificateur;
        }
    }

    if (carteDef.pouvoir.type == GAIN_VIE && carteDef.pouvoir.condition == DEFAITE && carteDef.pouvoirEstActifContre[numVic])
        *pvDef += carteDef.pouvoir.modificateur;

        //bonus
    if (carteVic.bonus.type == GAIN_VIE && carteVic.bonusEstActifContre[numDef])
        *pvVic += carteVic.bonus.modificateur;

//perte vie
        //pouvoir
    if (carteVic.pouvoir.type == PERTE_VIE)
    {
        if (carteVic.pouvoirEstActifContre[numDef] && carteVic.pouvoir.condition != EN_CAS_DE_STOP)
        {
            if (carteVic.pouvoir.condition != DEFAITE && *pvDef > carteVic.pouvoir.minimum)
            {
                *pvDef += carteVic.pouvoir.modificateur;

                if (*pvDef < carteVic.pouvoir.minimum)
                    *pvDef = carteVic.pouvoir.minimum;
            }
        }

        else if (carteVic.pouvoir.condition == EN_CAS_DE_STOP)
        {
            if (*pvDef > carteVic.pouvoir.minimum)
            {
                *pvDef += carteVic.pouvoir.modificateur;

                if (*pvDef < carteVic.pouvoir.minimum)
                    *pvDef = carteVic.pouvoir.minimum;
            }
        }
    }

    if (carteDef.pouvoir.type == PERTE_VIE && carteDef.pouvoir.condition == DEFAITE && carteDef.pouvoirEstActifContre[numVic] && *pvVic > carteDef.pouvoir.minimum)
    {
        *pvVic += carteDef.pouvoir.modificateur;

        if (*pvVic < carteDef.pouvoir.minimum)
            *pvVic = carteDef.pouvoir.minimum;
    }

        //bonus
    if (carteVic.bonus.type == PERTE_VIE && carteVic.bonusEstActifContre[numDef] && *pvDef > carteVic.bonus.minimum)
    {
        *pvDef += carteVic.bonus.modificateur;

        if (*pvDef < carteVic.bonus.minimum)
            *pvDef = carteVic.bonus.minimum;
    }
}


void rajouterVictoires()
{
    if (furyUtilisee)
        carteAlliee[persoTest].victoiresAvecXpillzEtFury[pillzTest]++;
    else
        carteAlliee[persoTest].victoiresAvecXpillzEntreCrochets[pillzTest]++;
}

void rajouterDefaites()
{
    if (furyUtilisee)
        carteAlliee[persoTest].defaitesAvecXpillzEtFury[pillzTest]++;
    else
        carteAlliee[persoTest].defaitesAvecXpillz[pillzTest]++;
}

void rajouterEgalites()
{
    if (furyUtilisee)
        carteAlliee[persoTest].egalitesAvecXPillzEtFury[pillzTest]++;
    else
        carteAlliee[persoTest].egalitesAvecXPillz[pillzTest]++;
}

bool jeRemporteLeRound(int resultat, int round)
{
    if (resultat == VAINQUEUR)
        return true;
    else if (resultat == PERDANT)
        return false;
    else
        return egaliteDAttaque(round);
}

int oracle(int pv, int pillz, int pvE, int pillzE) {
	return (pv+pillz) - (pvE-pillzE);
}


void calculerRound(int nRound, int pv, int pvEnnemis, int pillz, int pillzEnnemis, bool determinQuiEstTeste, int carteEnnemieEnvoyee, int dernierRoundACalculer) {
    int pillzRestants, pillzEnnemisRestants;
    bool jeGagne = false;
    
    if (nRound == dernierRoundACalculer+1) {
    	if (furyUtilisee) {
    		carteAlliee[persoTest].guessedScoreFury[pillzTest] 	+= oracle(pv, pillz, pvEnnemis, pillzEnnemis);
    	} else {
    		carteAlliee[persoTest].guessedScore[pillzTest]		+= oracle(pv, pillz, pvEnnemis, pillzEnnemis);
    	}
    	
    	return;
    }

    if (nRound == 5 || pvEnnemis <= 0 || pv <= 0) { // Combat terminé 
        if (pv <= 0 && pvEnnemis <= 0)
            rajouterEgalites();
        else if (pv > pvEnnemis)
            rajouterVictoires();
        else if (pv < pvEnnemis)
            rajouterDefaites();
        else
            rajouterEgalites();
    }
    else {
        for (int i = 0 ; i < 4 ; i++) { // Pour chaque carte alliée
            if (!carteAlliee[i].supposeeUtilisee && !carteAlliee[i].utiliseeACoupSur) { // ne continuer que si elle n'est pas supposée utilisée, ni pour de bon
                if (determinQuiEstTeste)
                    persoTest = i;
                carteAlliee[i].supposeeUtilisee = true; // la supposer utilisée
                for (int j = 0 ; j < 4 ; j++) { // Pour chaque carte ennemie
                    if (!carteEnnemie[j].supposeeUtilisee && !carteEnnemie[j].utiliseeACoupSur) { // ne continuer que si elle n'est pas supposée utilisée, ni pour de bon
                        if (!(determinQuiEstTeste && carteEnnemieEnvoyee >= 0 && carteEnnemieEnvoyee <= 3) || j == carteEnnemieEnvoyee) { // En simplifiant par la logique, on obtient ça
                        //TODO vérifier la condition, elle est bizarre, et mettre un commentaire qui résume mieux celle-ci. De plus, "compris entre 0 et 3", ça veut dire "différent de -1", non ?
                        // on ne continue que si j = la carte ennemie envoyée à coup sûr, ou qu'on s'en fout (= on teste toutes celles qui restent)
                            carteEnnemie[j].supposeeUtilisee = true;
                            for (int k = 0 ; k <= pillz ; k++) {// Pour chaque pillz
                                pillzRestants = pillz - k;

                                if (determinQuiEstTeste)
                                    pillzTest = k;
                                for (int l = 0 ; l <= pillzEnnemis ; l++) { // Pour chaque pillz ennemi
                                    pillzEnnemisRestants = pillzEnnemis - l;
                                    jeGagne = jeRemporteLeRound(carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l], nRound);

                                    modifPillzPvEtRoundSuivant(pv, pvEnnemis, pillzRestants, pillzEnnemisRestants, i, j, nRound, jeGagne, false, false, dernierRoundACalculer);

                                    if (pillzRestants >= 3) {                                    
                                        if (determinQuiEstTeste)
                                            furyUtilisee = true;

                                        modifPillzPvEtRoundSuivant(pv, pvEnnemis, pillzRestants - 3, pillzEnnemisRestants , i, j, nRound, jeGagne, true, false, dernierRoundACalculer);

                                        if (pillzEnnemisRestants >= 3)
                                            modifPillzPvEtRoundSuivant(pv, pvEnnemis, pillzRestants - 3, pillzEnnemisRestants - 3, i, j, nRound, jeGagne, true, true, dernierRoundACalculer);

                                        if (determinQuiEstTeste)
                                            furyUtilisee = false;
                                    }
                                    if (pillzEnnemisRestants >= 3)
                                        modifPillzPvEtRoundSuivant(pv, pvEnnemis, pillzRestants, pillzEnnemisRestants - 3, i, j, nRound, jeGagne, false, true, dernierRoundACalculer);
                                }
                            }
                            carteEnnemie[j].supposeeUtilisee = false; //desupposer
                        }
                    }
                }
                carteAlliee[i].supposeeUtilisee = false; // desupposer
            }
        }
    }
}


void modifPillzPvEtRoundSuivant(int pv, int pvEnnemis, int pillz, int pillzEnnemis, int i, int j, int round, bool jeGagne, bool furyAllie, bool furyEnnemie, int dernierRoundACalculer)
{
    int pillzApresModif = pillz, pillzEnnemisApresModif = pillzEnnemis, pvApresModif = pv, pvEnnemisApresModif = pvEnnemis;
    if (jeGagne)
    {
        modifPerteGainVie(i, &pvApresModif, carteAlliee[i], j, &pvEnnemisApresModif, carteEnnemie[j], furyAllie);
        modifPerteGainPillz(i, &pillzApresModif, carteAlliee[i], j, &pillzEnnemisApresModif, carteEnnemie[j]);
    }

    else
    {
        modifPerteGainVie(j, &pvEnnemisApresModif, carteEnnemie[j], i, &pvApresModif, carteAlliee[i], furyEnnemie);
        modifPerteGainPillz(j, &pillzEnnemisApresModif, carteEnnemie[j], i, &pillzApresModif, carteAlliee[i]);
    }

    calculerRound(round + 1, pvApresModif, pvEnnemisApresModif, pillzApresModif, pillzEnnemisApresModif, false, -1, dernierRoundACalculer);
}


