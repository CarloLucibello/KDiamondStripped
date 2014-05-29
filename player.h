#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "rand.h"



class Player{
public:
    Player(Game * game, int seed = -1);

    void playMove(int i, bool verbose = false);
    void playRandomMove(bool verbose = false);
    void playSmartestMove( bool verbose = false); // va adattata di volta in volta alle regole del gioco

    //estraggo un numero random tra 0 e 1 e scelgo se fare la mossa più intelligente in base al qi del giocatore
    void playFuzzyMove(double qi, bool verbose = false);

private:
    Game * m_game;
    LinCongRNG m_rng;

};

#endif // PLAYER_H
