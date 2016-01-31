#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"
/// /// Si l'ennemi commence le round, il ne faut tester que cette carte là !
///on ne peut calculer que les furys de pillzrestants-3 ! quand j'ai 4 pillz il me dit : avec 3 pillz+fury = 76 victoires...
/// a changer de partout si pas deja fait ! "faire si le pouvoir n'est pas stoppé ET qu'il n'est pas de type en cas de stop"....
///mettre le coup du kbhit pour le "a mis la fury ?" et affichage victoires

// n'affiche pas le meme pouvoir pour thormund chez ennemi si un des 4 != junta - T'es sur?
// petits problemes si pluseurs fois meme carte avec carteCourage
// qui s'active en premier ? le poison ou le berzerk?
// la variable nomInexistant se retrouve dans plusieurs fonctions. pas moyen de factoriser la vérification ?


//on peut passer des pointeurs sur les grosses structures plutot que de les passer entierement. a faire plutôt dans les fonctions simples et appelées souvent
//pas que dans ce fichier : partout ou il y a des paquets de lignes ABSOLUMENT identiques, on peut faire une fonction à la place. En particulier les cas d'"égalité" qui doublent le contenu.
//est-ce qu'il y a des endroits ou on calcule plusieurs fois une même valeur, alors qu'on pourrait la stocker en mémoire?
//pour aller plus vite : créer des variables (globales, probablement...) qui retiennent que le duo posséde des trucs qui touchent à la vie (ou aux pillz) (ou pas), on fera les modifs que si la variable vaut true
//le calcul 1er tour OS+fury fera forcement moins de victoires au compteur qu'une sasha 2 pillz qui va, elle, aligner des victoires sur les 4 rounds.... comment y remédier? est-ce grave?
/*
--------TRES IMPORTANT--------
tu gagneras si tu reponds comme il faut aux attaques (si ce n'est pas fait, il y a aura moins souvent de possibilites victorieuses) - a verifier - et est-ce qu'il le fait pas déjà?

--------IMPORTANT-------------
modifier le style d'indentation pour prendre moins de place
privilegier l'egalite a la defaite
vortex-like !!!
poison et regen
defaite +X vie se fait après les degats, donc si tu meurs pas de RES
courage, revanche, confiance
frozn
verifier les conditions pour passer aux autres fonctions, les victoires++ et les arguments envoyes, j'ai supprime plein de conditions pour que le programme aille plus vite. Avec ce genre de pouvoirs, on peut gagner dans des situations totalement farfelues... pour l'instant, le programme suit un raisonnement tres simpliste...

--------EVENTUELLEMENT--------
accelerer encore les rounds 1 ( il y a de petites optimisations simples proposées en commentaires)
virer les includes inutiles
printf, scanf -> cout, cin
leaders
pouvoir choisir entre égalité à coup sûr et continuer le calcul normalement
ne pas demander "qui?" si les 4 sont la même carte
contrecoup - encore que peu courant
demander si c'est en elo(14 pv) ou pas. Bon ok a priori ce programme servira uniquement pour le elo...
gerer plus que 12 pillz (si jamais nobrodroid passe 1er tour avec moins de 3 pillz...)
enlever les variables globales
si on peut gagner de plusieurs manieres, choisir celle qui se remarquera le moins par l'adversaire, a savoir celle qui permet de gagner avec un ecart de pv tres faible, pour qu'il se doute moins de la chose
si j'ai plusieurs fois le meme perso ,ne pas recalculer a chaque fois, mais copier
changer les printf pour faire un truc un peu sympa, et eviter les questions stupides genre "et il envoie qui pour le 4eme round?", afficher "machin a battu machin (et a inflige tant-ca va se compliquer avec les poisons, les recups et tout-)", et enfin mettre des effacerEcran pour que ce soit plus simple a suivre   - et peut-etre meme des sleep ! -

--------ET PEUT-ETRE MEME, UN JOUR, QUAND JE SERAI RICHE ET DOMINANT PLEIN D'EMPLOYES (ou pas) -------------
creer un deck-maker pour le elo, soyons fous \o/
mettre les semi-evos les plus courantes (wanda, burt, deebler...)  (toutes?)
si plusieurs façons de gagner à coup sur, prendre la plus rapide ^^
Mettre toutes les cartes en mémoire, même les plus nulles
Mettre tout ca en C++, avec Qt !  =D

--------TRUCS PAS CLAIRS------
vérifier si pas de doublons dans le perso.txt, ce serait du gachis
faut-il calculer les possibilites de defaite? (et faire ensuite une soustraction?)"ah ouais comme ca tu peux peut etre gagner..." et pam kolos fury DTC (-> he c'est la vie ca vieux)  => +2 variables : egaliteacXpillz et defaitesacXpillz
virer les variables inutiles !! (voire fonctions...) (et bien sur commentaires...)
s'assurer du bon fonctionnement des fonctions...
HS : c'est bien un {0} pour initialiser les tableaux doubles aussi??
*/
Carte carteAlliee[4];
Carte carteEnnemie[4];// ben oui variables globales, mais bon on les utilise tellement que c'est plus pratique...
int pillzTest = 0, persoTest = 0;
bool ennemiCommence = false, furyUtilisee = false;

int main(void)
{
    int pillz = 12, pillzAdverses = 12, pointsDeVie = 14, pointsDeVieAdverses = 14;

    lectureDesFichiers();
    for (int i = 0 ; i < 4 ; i++)
        {
            carteAlliee[i].utiliseeACoupSur = 0;
            carteEnnemie[i].utiliseeACoupSur = 0;
            for (int j = 0 ; j < 4 ; j++)
                {
                    for (int k = 0 ; k < pillz ; k++)
                        {
                            for (int l = 0 ; l < pillzAdverses ; l++)
                                carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = NEANT;
                        }
                }
        }
    calculDesDuels(pillz, pillzAdverses);
    calculDesDegats();

    remiseAZero();//pour eviter les valeures farfelues...

    whatAboutPersos(carteAlliee);
    whatAboutPersos(carteEnnemie);

    quiCommenceSelonLesEtoiles();

    processGame(pillz, pillzAdverses, pointsDeVie, pointsDeVieAdverses);
//je crée ces variables dans le main pour quand on aura à demander des trucs qui n'ont rien a faire dans la fonction (ELO ou pas...), mieux vaut appeler des fonctions depuis le main.

    //je mets le programme en boucle ou on le relance a chaque fois? Il sera peut-être plus rapide la 2ème fois ? =D
    printf("\nPress Enter to quit\n");
    getchar();

    return EXIT_SUCCESS;
}


