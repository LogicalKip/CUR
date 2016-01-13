#ifndef DEF_URBANMCTS
#define DEF_URBANMCTS

#include <set>

#include "game.hpp"
#include "constantes.hpp"



struct UrbanRivalsState
{
	int pvEnnemis = 14;//FIXME constantes
	int pvAllies = 14;
	int pillzEnnemis = 12;
	int pillzAllies = 12;
	int roundsTermines = 0;
	bool ennemiCommence;
	Carte* carteAlliee;
	Carte* carteEnnemie;

	// pour traiter les coups en réponse à la carte envoyée en courage
	bool attendCarteReponse = false;
	int carteCourage; // inutiles si !attendCarteReponse
	int pillzCourage;
	bool furyCourage;
};
	
class UrbanRivals : public game::game<UrbanRivalsState>
{
	public:
		UrbanRivals(Carte allie[4], Carte ennemi[4], bool ennemiCommence);//FIXME init state (dont les pointeurs sur les tableaux de Carte)
		UrbanRivals(const UrbanRivals& ur);

		bool end_of_game() const;
		int value(std::uint8_t player) const;
		bool won(std::uint8_t player) const;
		bool lost(std::uint8_t player) const;
		bool draw(std::uint8_t player) const;
		uint8_t current_player() const;
		std::uint16_t number_of_moves() const;
		void play(std::uint16_t m);
		void undo(std::uint16_t m);
		std::string player_to_string(std::uint8_t player) const;
		std::string move_to_string(std::uint16_t m) const;
		std::string to_string() const;
		std::set<int> to_input_vector() const; 
		void from_input_vector(const std::set<int>& input);
		UrbanRivalsState get_state();
		void set_state(const UrbanRivalsState& s);
		std::shared_ptr<game<UrbanRivalsState>> do_copy() const;
		std::uint64_t hash(std::uint16_t m) const;
		std::uint64_t hash() const;
		
	private:
		int getPillz(uint8_t player) const;
		void getMoveFromInt(const int m, const std::vector<int> cartesDispo, const int pillzRestant, int& carteAJouer, int& pillzAJouer, bool& fury) const;
		bool decryptedMove(const int m, const int compteur, int& carteAJouer, int& pillzAJouer, bool& fury, const int c, const int p, const bool f) const;
		Carte* getCurrentPlayerCards();		

		UrbanRivalsState state;   

		Carte carteAlliee[4];
		Carte carteEnnemie[4];

		const uint8_t JOUEUR_ALLIE = 0;
		const uint8_t JOUEUR_ENNEMI = JOUEUR_ALLIE + 1;
};


#endif
