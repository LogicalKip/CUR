#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/lectureFichiers.hpp"
#include "../include/main.hpp"


bool pouvoirActif(Carte carteATester, Carte adversaire)//ATTENTION !!! qu'on se le dise : pouvoir actif veut certes dire non stoppe, mais aussi qu'il est different de stop bonus/pouvoir, protection et autres !!! D'accord le nom de la fonction ne le laisse pas deviner, mais vas-y trouve moi un nom qui resume ce commentaire en quelques mots...
{//reverifier cette fonction de temps en temps ne peut pas faire de mal, elle est compliquee...
    bool res = false;
    if (carteATester.pouvoir.type != STOP_BONUS && carteATester.pouvoir.type != STOP_POUVOIR && carteATester.pouvoir.type != PROTECTION_BONUS && carteATester.pouvoir.type != NEANT)
        {
            if(adversaire.pouvoir.type == STOP_POUVOIR)//epsp
                {
                    if(carteATester.bonus.type == STOP_POUVOIR)//absp
                        res = !(adversaire.bonus.type == STOP_BONUS || adversaire.bonus.type == PROTECTION_POUVOIR);
                    else if (carteATester.bonus.type == PROTECTION_POUVOIR)//abPrp
                        res = !(adversaire.bonus.type == STOP_BONUS);
                }

            else if (adversaire.bonus.type == STOP_POUVOIR)//ebsp
                {
                    if (carteATester.bonus.type == STOP_BONUS)//absb
                        res = !(adversaire.pouvoir.type == STOP_BONUS || adversaire.pouvoir.type == PROTECTION_BONUS);
                    else if (carteATester.bonus.type == PROTECTION_POUVOIR)//abprp
                        res = !(adversaire.pouvoir.type == STOP_BONUS);
                }
            else
                res = true;
        }
    return res;
}

bool bonusActif(Carte carteATester, Carte adversaire)//ATTENTION !!! qu'on se le dise : bonus actif veut certes dire non stoppe, mais aussi qu'il est different de stop bonus/pouvoir, protection et autres !!! D'accord le nom de la fonction ne le laisse pas deviner, mais vas-y trouve moi un nom qui resume ce commentaire en quelques mots...
{
    bool res = false;
    if (carteATester.bonus.type != STOP_BONUS && carteATester.bonus.type != STOP_POUVOIR && carteATester.bonus.type != PROTECTION_POUVOIR && carteATester.bonus.type != NEANT)
        {
            if (adversaire.pouvoir.type == STOP_BONUS)//epsb
                {
                    if (carteATester.pouvoir.type == STOP_POUVOIR)//apsp
                        res = !(adversaire.bonus.type == STOP_POUVOIR || adversaire.bonus.type == PROTECTION_POUVOIR);
                    else if (carteATester.pouvoir.type == PROTECTION_BONUS)//apprb
                        res = !(adversaire.bonus.type == STOP_POUVOIR);
                }

            else if (adversaire.bonus.type == STOP_BONUS)//ebsb
                {
                    if (carteATester.pouvoir.type == PROTECTION_BONUS)//apprb
                        res = ! (adversaire.pouvoir.type == STOP_POUVOIR);
                    else if (carteATester.pouvoir.type == STOP_BONUS)//apsb
                        res = !(adversaire.pouvoir.type == STOP_POUVOIR || adversaire.pouvoir.type == PROTECTION_BONUS);
                }
            else
                res = true;
        }
    return res;
}


void determinerDegats(Carte *carteATester, Carte adversaire, int numCarteATester, int numAdversaire)
{
    carteATester->degatsContre[numAdversaire] = carteATester->degatsDeBase;
    carteATester->degatsAvecFuryContre[numAdversaire] = carteATester->degatsDeBase;

    if (carteATester->pouvoir.type == COPIER_DEGATS && carteATester->pouvoirEstActifContre[numAdversaire])
        {
            carteATester->degatsContre[numAdversaire] = adversaire.degatsDeBase;
            carteATester->degatsAvecFuryContre[numAdversaire] = adversaire.degatsDeBase;
        }

    if (carteATester->pouvoir.type == AUGMENTER_DEGATS)
        {
            if (carteATester->pouvoirEstActifContre[numAdversaire])
                {
                    if (carteATester->pouvoir.condition != EN_CAS_DE_STOP)
                        {
                            carteATester->degatsContre[numAdversaire] += carteATester->pouvoir.modificateur;
                            carteATester->degatsAvecFuryContre[numAdversaire] += carteATester->pouvoir.modificateur;
                        }
                }

            else if (carteATester->pouvoir.condition == EN_CAS_DE_STOP)
                {
                    carteATester->degatsContre[numAdversaire] += carteATester->pouvoir.modificateur;
                    carteATester->degatsAvecFuryContre[numAdversaire] += carteATester->pouvoir.modificateur;
                }
        }

    if (carteATester->bonus.type == AUGMENTER_DEGATS && carteATester->bonusEstActifContre[numAdversaire])
        {
            carteATester->degatsContre[numAdversaire] += carteATester->bonus.modificateur;
            carteATester->degatsAvecFuryContre[numAdversaire] += carteATester->bonus.modificateur;
        }

    carteATester->degatsAvecFuryContre[numAdversaire] += 2;

    if (adversaire.pouvoir.type == DIMINUER_DEGATS)
        {
            if ((adversaire.pouvoirEstActifContre[numCarteATester] && adversaire.pouvoir.condition != EN_CAS_DE_STOP) || (!adversaire.pouvoirEstActifContre[numCarteATester] && adversaire.pouvoir.condition == EN_CAS_DE_STOP))
                {
                    if(carteATester->pouvoir.type != PROTECTION_DEGATS || ! carteATester->pouvoirEstActifContre[numAdversaire])
                        {
                            if (carteATester->degatsContre[numAdversaire] > adversaire.pouvoir.minimum)
                                {
                                    carteATester->degatsContre[numAdversaire] += adversaire.pouvoir.modificateur;

                                    if (carteATester->degatsContre[numAdversaire] < adversaire.pouvoir.minimum)
                                        carteATester->degatsContre[numAdversaire] = adversaire.pouvoir.minimum;
                                }

                            if (carteATester->degatsAvecFuryContre[numAdversaire] > adversaire.pouvoir.minimum)
                                {
                                    carteATester->degatsAvecFuryContre[numAdversaire] += adversaire.pouvoir.modificateur;

                                    if (carteATester->degatsAvecFuryContre[numAdversaire] < adversaire.pouvoir.minimum)
                                        carteATester->degatsAvecFuryContre[numAdversaire] = adversaire.pouvoir.minimum;
                                }
                        }
                }
        }

    if (adversaire.bonus.type == DIMINUER_DEGATS)
        {
            if (adversaire.bonusEstActifContre[numCarteATester])
            {
                if (carteATester->pouvoir.type != PROTECTION_DEGATS || ! carteATester->pouvoirEstActifContre[numAdversaire])
                    {
                        if (carteATester->degatsContre[numAdversaire] > adversaire.bonus.minimum)
                            {
                                carteATester->degatsContre[numAdversaire] += adversaire.bonus.modificateur;

                                if (carteATester->degatsContre[numAdversaire] < adversaire.bonus.minimum)
                                    carteATester->degatsContre[numAdversaire] = adversaire.bonus.minimum;
                            }

                        if (carteATester->degatsAvecFuryContre[numAdversaire] > adversaire.bonus.minimum)
                            {
                                carteATester->degatsAvecFuryContre[numAdversaire] += adversaire.bonus.modificateur;

                                if (carteATester->degatsAvecFuryContre[numAdversaire] < adversaire.bonus.minimum)
                                    carteATester->degatsAvecFuryContre[numAdversaire] = adversaire.bonus.minimum;
                            }
                    }
            }
        }
}

void calculDesDegats()
{
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0 ; j < 4 ; j++)
            {
                determinerDegats(&carteAlliee[i], carteEnnemie[j], i, j);
                determinerDegats(&carteEnnemie[j], carteAlliee[i], j, i);
            }
    }
}

void augmenterPuissance(Carte *carteATester, Carte adversaire, int numAdversaire)
{
    if (carteATester->pouvoir.type == COPIER_PUISSANCE && carteATester->pouvoirEstActifContre[numAdversaire])
        carteATester->puissanceTemporaire = adversaire.puissance;

    if (carteATester->pouvoir.type == AUGMENTER_PUISSANCE)
        {
            if (carteATester->pouvoirEstActifContre[numAdversaire])
                {
                    if (carteATester->pouvoir.condition != EN_CAS_DE_STOP)
                        carteATester->puissanceTemporaire = carteATester->puissanceTemporaire + carteATester->pouvoir.modificateur;
                }

            else if (carteATester->pouvoir.condition == EN_CAS_DE_STOP)
                carteATester->puissanceTemporaire = carteATester->puissanceTemporaire + carteATester->pouvoir.modificateur;
        }

    if (carteATester->bonus.type == AUGMENTER_PUISSANCE && carteATester->bonusEstActifContre[numAdversaire])
        carteATester->puissanceTemporaire = carteATester->puissanceTemporaire + carteATester->bonus.modificateur;
}

void diminuerPuissance(Carte carteATester, Carte *adversaire, int numCarteATester, int numAdversaire)
{
    if (carteATester.pouvoir.type == DIMINUER_PUISSANCE)
        {
            if (carteATester.pouvoirEstActifContre[numAdversaire])
                {
                    if (carteATester.pouvoir.condition != EN_CAS_DE_STOP)
                        {
                            if (adversaire->pouvoir.type != PROTECTION_PUISSANCE || adversaire->pouvoirEstActifContre[numCarteATester] == 0)
                                {
                                    adversaire->puissanceTemporaire = adversaire->puissanceTemporaire + carteATester.pouvoir.modificateur;// + car le modificateur est un nombre negatif

                                    if (adversaire->puissanceTemporaire < carteATester.pouvoir.minimum)
                                        adversaire->puissanceTemporaire = carteATester.pouvoir.minimum;
                                }
                        }
                }

            else if (carteATester.pouvoir.condition == EN_CAS_DE_STOP)
                {
                    if (adversaire->pouvoir.type != PROTECTION_PUISSANCE || adversaire->pouvoirEstActifContre[numCarteATester] == 0)
                        {
                            adversaire->puissanceTemporaire = adversaire->puissanceTemporaire + carteATester.pouvoir.modificateur;// + car le modificateur est un nombre negatif

                            if (adversaire->puissanceTemporaire < carteATester.pouvoir.minimum)
                                adversaire->puissanceTemporaire = carteATester.pouvoir.minimum;
                        }
                }
        }

    if (carteATester.bonus.type == DIMINUER_PUISSANCE && carteATester.bonusEstActifContre[numAdversaire])
        {
            if (adversaire->pouvoir.type != PROTECTION_PUISSANCE || adversaire->pouvoirEstActifContre[numCarteATester] == 0)
                {
                    adversaire->puissanceTemporaire = adversaire->puissanceTemporaire + carteATester.bonus.modificateur;

                    if (adversaire->puissanceTemporaire < carteATester.bonus.minimum)
                        adversaire->puissanceTemporaire = carteATester.bonus.minimum;
                }
        }
}

void modifierAttaque(int *attaque, Carte carteAModifier, Carte carteAffrontee, int numCarteAModifier, int numAdversaire)
{
    if (carteAModifier.pouvoir.type == AUGMENTER_ATTAQUE)//augmenter d'abord
        {
            if (carteAModifier.pouvoirEstActifContre[numAdversaire])
                {
                    if (carteAModifier.pouvoir.condition != EN_CAS_DE_STOP)
                        *attaque = *attaque + carteAModifier.pouvoir.modificateur;
                }


            else if (carteAModifier.pouvoir.condition == EN_CAS_DE_STOP)
                *attaque = *attaque + carteAModifier.pouvoir.modificateur;
        }


    if (carteAModifier.bonus.type == AUGMENTER_ATTAQUE && carteAModifier.bonusEstActifContre[numAdversaire])
        *attaque = *attaque + carteAModifier.bonus.modificateur;

    if (carteAffrontee.bonus.type == DIMINUER_ATTAQUE && *attaque > carteAffrontee.bonus.minimum && carteAffrontee.bonusEstActifContre[numCarteAModifier])
        {
            *attaque = *attaque + carteAffrontee.bonus.modificateur;//+ car il s'agit d'un nombre NEGATIF !

            if (*attaque < carteAffrontee.bonus.minimum)
                *attaque = carteAffrontee.bonus.minimum;
        }

    if (carteAffrontee.pouvoir.type == DIMINUER_ATTAQUE)//baisser ensuite
        {
            if (*attaque > carteAffrontee.pouvoir.minimum)
                {
                    if (carteAffrontee.pouvoirEstActifContre[numCarteAModifier])
                        {
                            *attaque = *attaque + carteAffrontee.pouvoir.modificateur;//+ car il s'agit d'un nombre NEGATIF !

                            if (*attaque < carteAffrontee.pouvoir.minimum)
                                *attaque = carteAffrontee.pouvoir.minimum;
                        }

                    else if (carteAffrontee.pouvoir.condition == EN_CAS_DE_STOP)
                        {
                            *attaque = *attaque + carteAffrontee.pouvoir.modificateur;//+ car il s'agit d'un nombre NEGATIF !

                            if (*attaque < carteAffrontee.pouvoir.minimum)
                                *attaque = carteAffrontee.pouvoir.minimum;
                        }
                }
        }
}

void calculDesDuels(int pillz, int pillzAdverses)//attention ! quand interviendront les conditions d'activation des pouvoirs (courage, revanche...), on ne pourra plus le calculer a l'avance ! Que faire?
{
    int attaqueAlliee = 0, attaqueEnnemie = 0;

    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0 ; j < 4 ; j++)
            {
                carteAlliee[i].pouvoirEstActifContre[j] = pouvoirActif(carteAlliee[i], carteEnnemie[j]);
                carteEnnemie[j].pouvoirEstActifContre[i] = pouvoirActif(carteEnnemie[j], carteAlliee[i]);
                carteAlliee[i].bonusEstActifContre[j] = bonusActif(carteAlliee[i], carteEnnemie[j]);
                carteEnnemie[j].bonusEstActifContre[i] = bonusActif(carteEnnemie[j], carteAlliee[i]);

                carteAlliee[i].puissanceTemporaire = carteAlliee[i].puissance;
                carteEnnemie[j].puissanceTemporaire = carteEnnemie[j].puissance;


                augmenterPuissance(&carteAlliee[i], carteEnnemie[j], j);
                augmenterPuissance(&carteEnnemie[j], carteAlliee[i], i);

                diminuerPuissance(carteAlliee[i], &carteEnnemie[j], i, j);
                diminuerPuissance(carteEnnemie[j], &carteAlliee[i], j, i);

                for (int k = 0 ; k <= pillz ; k++)
                    {
                        attaqueAlliee = attaque(carteAlliee[i].puissanceTemporaire, k);

                        modifierAttaque(&attaqueAlliee, carteAlliee[i], carteEnnemie[j], i, j);

                        for (int l = 0 ; l <= pillzAdverses ; l++)
                            {
                                attaqueEnnemie = attaque(carteEnnemie[j].puissanceTemporaire, l);

                                modifierAttaque(&attaqueEnnemie, carteEnnemie[j], carteAlliee[i], j, i);

                                if (attaqueAlliee > attaqueEnnemie)
                                    carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = VAINQUEUR;

                                else if (attaqueAlliee < attaqueEnnemie)
                                    carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = PERDANT;

                                else
                                    {
                                        if (carteAlliee[i].nombreDEtoiles > carteEnnemie[j].nombreDEtoiles)
                                            carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = PERDANT;

                                        else if (carteAlliee[i].nombreDEtoiles < carteEnnemie[j].nombreDEtoiles)
                                            carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = VAINQUEUR;

                                        else
                                            carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = EGALITE;//on determinera ça au courage
                                    }
                            }
                    }
            }
    }
}


