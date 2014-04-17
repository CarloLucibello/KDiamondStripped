#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "rand.h"



class Player{
public:
    Game * m_game;
    LinCongRNG rng;
    //ParRap rng;

    Player(Game * game, int seed = -1) : m_game(game){
        rng.setSeed(seed);
    };

    void playRandomMove(){
        const auto& moves = m_game->availMoves();

        auto m = moves[rng.unifInt(moves.size())];

        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);
        cout << "MOVED :  " << m.first.x() << "  " << m.first.y() << " --> " << m.second.x() << "  " << m.second.y() <<  endl;
    }

};

#endif // PLAYER_H
