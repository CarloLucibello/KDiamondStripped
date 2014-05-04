#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "rand.h"



class Player{
    double qi;
public:
    Game * m_game;
    LinCongRNG rng;

    Player(Game * game, int seed = -1) : m_game(game){
        rng.setSeed(seed);
    };

    void playRandomMove(bool verbose = false){
        const auto& moves = m_game->availMoves();
        playMove(rng.unifInt(moves.size()), verbose);
    }

    void playMove(int i, bool verbose){
        const auto& moves = m_game->availMoves();
        auto m = moves[i % moves.size()];
        if(verbose){
            cout << "MOVING : " << m.from().x() << " " << m.from().y() << " --> " << m.to().x() << " " << m.to().y() << endl;
            cout << endl;
        }
        m_game->clickDiamond(m.from());
        m_game->clickDiamond(m.to());
   }

    void playSmartRandomMove(double qi, bool verbose = false){
        const auto& moves = m_game->availMoves();

        int argmax = -1;
        int max = 0;
        for(int i = 0; i < moves.size(); ++i){
            const auto& m = moves[i];
            if(m.numToDelete() > max){
                max = m.numToDelete();
                argmax = i;
            }
        }

        //estraggo un numero random tra 0 e 1 e scelgo se fare la mossa più intelligente in base al qi del giocatore
        double r = rng.unifReal();
        int mossa_scelta = r < qi ? argmax : rng.unifInt(moves.size());
        playMove(mossa_scelta, verbose);
    }
};

#endif // PLAYER_H
