#include <iostream>

#include "../include/constantes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include "../include/autresFonctions.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/calculPrealable.hpp"
#include "../include/lectureFichiers.hpp"

using std::cout;

/// a changer de partout si pas deja fait ! "faire si le pouvoir n'est pas stoppé ET qu'il n'est pas de type en cas de stop"....
///mettre le coup du kbhit pour le "a mis la fury ?" et affichage victoires

//on peut passer des pointeurs sur les grosses structures plutot que de les passer entierement. a faire plutôt dans les fonctions simples et appelées souvent
//est-ce qu'il y a des endroits ou on calcule plusieurs fois une même valeur, alors qu'on pourrait la stocker en mémoire?
//pour aller plus vite : créer des variables (globales, probablement...) qui retiennent que le duo posséde des trucs qui touchent à la vie (ou aux pillz) (ou pas), on fera les modifs que si la variable vaut true
//le calcul 1er tour OS+fury fera forcement moins de victoires au compteur qu'une sasha 2 pillz qui va, elle, aligner des victoires sur les 4 rounds.... comment y remédier? est-ce grave?
/*
--------TRES IMPORTANT--------
tu gagneras si tu reponds comme il faut aux attaques (si ce n'est pas fait, il y a aura moins souvent de possibilites victorieuses) - a verifier - et est-ce qu'il le fait pas déjà?

--------IMPORTANT-------------
privilegier l'egalite a la defaite
vortex-like !!!
poison et regen
defaite +X vie se fait après les degats, donc si tu meurs pas de RES
courage, revanche, confiance
contrecoup
frozn
verifier les conditions pour passer aux autres fonctions, les victoires++ et les arguments envoyes, j'ai supprime plein de conditions pour que le programme aille plus vite. Avec ce genre de pouvoirs, on peut gagner dans des situations totalement farfelues... pour l'instant, le programme suit un raisonnement tres simpliste...

--------EVENTUELLEMENT--------
modifier le style d'indentation pour prendre moins de place
accelerer encore les rounds 1 ( il y a de petites optimisations simples proposées en commentaires), sinon prog dynamique avec un hash de l'état courant
virer les includes inutiles
const et ref quand c'est possible
endl au lieu de \n
factoriser les entrées utilisateurs
unsigned int ?
run some tests/update code to manage several times the same card in my (opponent's) deck
leaders
pouvoir choisir entre égalité à coup sûr et continuer le calcul normalement
ne pas demander "qui?" si les 4 sont la même carte
demander si c'est en elo(14 pv) ou pas. Bon ok a priori ce programme servira uniquement pour le elo...
gerer plus que 12 pillz (si jamais nobrodroid passe 1er tour avec moins de 3 pillz...)
enlever les variables globales
si on peut gagner de plusieurs manieres, choisir celle qui se remarquera le moins par l'adversaire, a savoir celle qui permet de gagner avec un ecart de pv tres faible, pour qu'il se doute moins de la chose
si j'ai plusieurs fois le meme perso ,ne pas recalculer a chaque fois, mais copier
changer les printf pour faire un truc un peu sympa, et eviter les questions stupides genre "et il envoie qui pour le 4eme round?", afficher "machin a battu machin (et a inflige tant-ca va se compliquer avec les poisons, les recups et tout-)", et enfin mettre des effacerEcran pour que ce soit plus simple a suivre   - et peut-etre meme des sleep ! -

-------- AND THAT'S WHEN I HAVE REALLY NOTHING ELSE TO DO -------------
Elo deck-maker \o/
Include most common half-XP (wanda, burt, deebler...)  (all of them ?)
If several ways to win otherwise absolutely the same, choose the quickest
Store all the cards \o !
GUI

--------TRUCS PAS CLAIRS------
vérifier si pas de doublons dans le perso.txt, ce serait du gachis
qui s'active en premier ? le poison ou le berzerk?
faut-il calculer les possibilites de defaite? (et faire ensuite une soustraction?) "ah ouais comme ca tu peux peut etre gagner..." et pam kolos fury DTC (-> he c'est la vie ca vieux)  => +2 variables : egaliteacXpillz et defaitesacXpillz
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
                                carteAlliee[i].combatAvecXPillzContreYAvecZpillz[k][j][l] = NOT_YET_KNOWN;
                        }
                }
        }
    calculDesDuels(pillz, pillzAdverses);
    calculDesDegats();

    reset();

    whatAboutCards(carteAlliee);
    whatAboutCards(carteEnnemie);

    findOutFirstPlayer();

    processGame(pillz, pillzAdverses, pointsDeVie, pointsDeVieAdverses);

    quit();

    return EXIT_SUCCESS;
}
