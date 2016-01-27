
#ifndef DEF_FONCTIONSBOUCLES
#define DEF_FONCTIONSBOUCLES


int attaque(int puissance, int pillzUtilises);
bool egaliteDAttaque(int round);

void rajouterVictoires();
void rajouterDefaites();
void rajouterEgalites();

void jePerdsLeRound(int i, int j, int k, int l, int pillzRestants, int pillzRestantsEnnemis, int pointsDeVieRestants, int pointsDeVieRestantsEnnemis, int round, int calculerAvecFury, int calculerSansFury);
void jeGagneLeRound(int i, int j, int k, int l, int pillzRestants, int pillzRestantsEnnemis, int pointsDeVieRestants, int pointsDeVieRestantsEnnemis, int round, int calculerAvecFury, int calculerSansFury);
void calculerRound(int nRound, int pv, int pvEnnemis, int pillz, int pillzEnnemis, bool determinQuiEstTeste, int carteEnnemieEnvoyee, int dernierRoundACalculer);

void modifPerteGainPillz(int numVic, int *pillzVic, Carte carteVic, int numDef, int *pillzDef, Carte carteDef);
void modifPerteGainVie(int numVic, int *pvVic, Carte carteVic, int numDef, int *pvDef, Carte carteDef, bool furyVic);
void modifPillzPvEtRoundSuivant(int pv, int pvEnnemis, int pillz, int pillzEnnemis, int i, int j, int round, bool jeGagne, bool furyAllie, bool furyEnnemie, int dernierRoundACalculer);

bool jeRemporteLeRound(int resultat, int round);

#endif



