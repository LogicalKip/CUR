#include "../include/urban-mcts.hpp"
#include "../include/fonctionsAnnexes.hpp"
#include "../include/fonctionsBoucles.hpp"
#include <vector>
#include <cstring>

UrbanRivals::UrbanRivals(Carte allie[4], Carte ennemi[4], bool ennemiCommence) {
	state.carteAlliee = allie;
	state.carteEnnemie = ennemi;
	state.ennemiCommence = ennemiCommence;
}

UrbanRivals::UrbanRivals(const UrbanRivals& ur) {
	this->state = ur.state;
	memcpy(this->carteAlliee, ur.carteAlliee, sizeof(ur.carteAlliee));
	memcpy(this->carteEnnemie, ur.carteEnnemie, sizeof(ur.carteEnnemie));
	this->state.carteAlliee = this->carteAlliee;
	this->state.carteEnnemie = this->carteEnnemie;
}

bool UrbanRivals::end_of_game() const {
	return state.pvEnnemis <= 0 || state.pvAllies <= 0 || state.roundsTermines == 4;
}

int UrbanRivals::value(std::uint8_t player) const {
	const int pvPlayer = (player == JOUEUR_ALLIE)  ? state.pvAllies : state.pvEnnemis;
	const int pvOpp    = (player == JOUEUR_ENNEMI) ? state.pvAllies : state.pvEnnemis;

  if (state.roundsTermines == 4 || pvOpp <= 0 || pvPlayer <= 0) // Combat terminé
    {
        if (pvPlayer <= 0 && pvOpp <= 0)
            return 0;
        else if (pvPlayer > pvOpp)
            return 1;
        else if (pvPlayer < pvOpp)
            return -1;
        else
            return 0;
    } else { 
		return 0;
	}
}


bool UrbanRivals::won(std::uint8_t player) const {
	return value(player) == 1;
}
bool UrbanRivals::lost(std::uint8_t player) const {
	return value(player) == -1;
}
bool UrbanRivals::draw(std::uint8_t player) const {
	return value(player) == 0;
}



/*
TODO reverifier la table de vérité

round courant = impair = i
ennemiCommence = e
attendCarte = c
ennemiDoitJouerMaintenant = x

i e c  x

0 0 1  0
0 1 0  0

0 1 1  1
0 0 0  1
1 0 1  1
1 1 0  1

i <=> e xor c

1 0 0  0
1 1 1  0

*/
uint8_t UrbanRivals::current_player() const {//FIXME à clarifier, et surtout sans faire d'erreurs, et utiliser attendCarteReponse
	
	bool i = state.roundsTermines % 2 == 0;// En comptant les rounds de 1 à 4
	bool e = state.ennemiCommence;
	bool c = state.attendCarteReponse;

	bool ennemiProchainAJouer = (i == (e != c)); // i <=> e xor c

	if (ennemiProchainAJouer) 	return JOUEUR_ENNEMI;
	else						return JOUEUR_ALLIE;
}

std::uint16_t UrbanRivals::number_of_moves() const {
	const int cartesRestantes = 4 - state.roundsTermines;
	const int pillzRestants = getPillz(current_player());	
	int res = cartesRestantes * (pillzRestants + 1);

	if (pillzRestants >= 3) { // Fury
		res += cartesRestantes * (pillzRestants - 2);	
	}

	return res;
}

int UrbanRivals::getPillz(uint8_t player) const {
	return player == JOUEUR_ALLIE ? state.pillzAllies : state.pillzEnnemis;
}

Carte* UrbanRivals::getCurrentPlayerCards() {
	return current_player() == JOUEUR_ALLIE ? state.carteAlliee : state.carteEnnemie;
}

void UrbanRivals::play(std::uint16_t m) {
	const int pillzRestant = getPillz(current_player());
	int carteAJouer = -1, pillzAJouer = -1;
	bool fury;
	Carte* currentDeck = getCurrentPlayerCards();	
	std::vector<int> cartesDispo;
	
	for (int i = 0 ; i < 4 ; i++) {
		if (!currentDeck[i].supposeeUtilisee && !currentDeck[i].utiliseeACoupSur) {// ne garder que si elle n'est pas utilisée (simulation), ni pour de bon
			cartesDispo.push_back(i);		      
		}
	}

	
	getMoveFromInt(m, cartesDispo, pillzRestant, carteAJouer, pillzAJouer, fury);

	if (state.attendCarteReponse) {
		// TODO calculer duel en utilisant carte/pillz/fury(cachés) de l'autre, MaJ pv,pillz, cartes utilisees
		

		state.roundsTermines++;
	} else { // courage
		state.carteCourage = carteAJouer;
		state.pillzCourage = pillzAJouer;
		state.furyCourage = fury;
	}
//FIXME quand modifier les pillz dépensés en courage ?
	currentDeck[carteAJouer].supposeeUtilisee = true;
	state.attendCarteReponse = !state.attendCarteReponse;

	
	//FIXME l'IA ne doit pas connaitre/utiliser les pillz/fury cachées

	//TODO jouer le coup
	//TODO mettre à jour state et Cartes[]
}

void UrbanRivals::getMoveFromInt(const int m, const std::vector<int> cartesDispo, const int pillzRestant, int& carteAJouer, int& pillzAJouer, bool& fury) const {//FIXME vérifier que tous les nombres sont représentés (pas de "saut" dû à trop de compteur++ par exemple)
	int compteur = -2; // 0 est le 1er coup
	for (std::vector<int>::const_iterator it = cartesDispo.begin(); it != cartesDispo.end(); it++) { // Pour chaque carte dispo
		compteur++;
		for (int p = 0 ; p <= pillzRestant ; p++) { // Pour chaque pillz
			compteur++;

			if (decryptedMove(m, compteur, carteAJouer, pillzAJouer, fury, *it, p, false)) {
				return;				
			}
			
			if (pillzRestant - p >= 3) {
				compteur++;

				if (decryptedMove(m, compteur, carteAJouer, pillzAJouer, fury, *it, p, true)) {
					return;				
				}				
			}
		}
	}
}

bool UrbanRivals::decryptedMove(const int m, const int compteur, int& carteAJouer, int& pillzAJouer, bool& fury, const int c, const int p, const bool f) const {
	if (compteur == m) {
		carteAJouer = c;
		pillzAJouer = p;
		fury = f;
		return true;			
	} else {
		return false;
	}
}

std::string UrbanRivals::player_to_string(std::uint8_t player) const {
	if (player == JOUEUR_ENNEMI) {
		return "l'adversaire";
	} else {
		return "l'utilisateur";
	}
}


std::string UrbanRivals::move_to_string(std::uint16_t m) const { return player_to_string(current_player()) + " joue une certaine carte avec un certain nombre de pillz et peut-etre la fury (coup n°" + std::to_string(m) + "). Désolé :("; } //TODO

std::string UrbanRivals::to_string() const {
	return whatAboutPillzHP(state.pillzAllies, state.pillzEnnemis, state.pvAllies, state.pvEnnemis);//FIXME ajouter round et peut-être les cartes restantes
}


UrbanRivalsState UrbanRivals::get_state() {
	return state;
}

void UrbanRivals::set_state(const UrbanRivalsState& s) {
    state = s; 
}

std::shared_ptr<game::game<UrbanRivalsState>> UrbanRivals::do_copy() const {
    return std::shared_ptr<UrbanRivals>(new UrbanRivals(*this));
}

/* FIXME Inutile, virer du .cpp/.hpp et game.hpp si possible */
std::set<int> UrbanRivals::to_input_vector() const {return std::set<int>();}
void UrbanRivals::from_input_vector(const std::set<int>& input) {}
void UrbanRivals::undo(std::uint16_t m) {}
std::uint64_t UrbanRivals::hash(std::uint16_t m) const {return 0;}
std::uint64_t UrbanRivals::hash() const {return 0;}
