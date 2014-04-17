#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
//#include "rng.h"
#include "rand.h"
using namespace cpputils;

class Player{
public:
    Game * m_game;
    UInt8 seed;
    //ParRap rng;

    Player(Game * game, UInt8 p_seed) : m_game(game), seed(p_seed){};
    void playRandomMove(){
        const auto& moves = m_game->availMoves();
        //auto m = moves[unifInt(rng, moves.size())];
        
        UInt8 r=IntRand(seed);
        auto m = moves[r%moves.size()];
        seed=r;
        
        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);
        cout << "MOVED :  " << m.first.x() << "  " << m.first.y() << " --> " << m.second.x() << "  " << m.second.y() <<  endl;
    }




};

#endif // PLAYER_H
