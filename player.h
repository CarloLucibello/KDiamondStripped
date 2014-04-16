#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "rng.h"
using namespace cpputils;

class Player{
public:
    Game * m_game;
    ParRap rng;

    Player(Game * game) : m_game(game){};
    void playRandomMove(){
        const auto& moves = m_game->availMoves();
        auto m = moves[unifInt(rng, moves.size())];
        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);
        cout << "MOVED :  " << m.first.x() << "  " << m.first.y() << " --> " << m.second.x() << "  " << m.second.y() <<  endl;
    }




};

#endif // PLAYER_H
