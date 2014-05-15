#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "rand.h"



class Player{
public:
    Game * m_game;
    LinCongRNG rng;

    Player(Game * game, int seed = -1) : m_game(game){
        rng.setSeed(seed);
    }

    void playMove(int i, bool verbose = false){
        const auto& moves = m_game->availMoves();
        auto m = moves[i % moves.size()];
        if(verbose){
            cout << "MOVING : " << m.from().x() << " " << m.from().y() << " <--> " << m.to().x() << " " << m.to().y() << endl;
            cout << endl;
        }
        m_game->clickDiamond(m.from());
        m_game->clickDiamond(m.to());
   }

    void playRandomMove(bool verbose = false){
        const auto& moves = m_game->availMoves();
        playMove(rng.unifInt(moves.size()), verbose);
    }

    // va adattata di volta in volta alle regole del gioco
    // TODO: dirgli di scoppiare preferibilmente le gelatine e le liquirizie
    void playSmartestMove( bool verbose = false){
        const auto& moves = m_game->availMoves();
        int chosen = -1;
        int numDel = 0;
        int numJollies = 0;

        for(int i = 0; i < moves.size(); ++i){
            const auto& m = moves[i];
            if(!m.contains(JollyType::Cookie)){ //non lo voglio esplodere inutilmente
                if(m.numJollies() > numJollies){
                    numDel = m.numToDelete();
                    numJollies = m.numJollies();
                    chosen = i;
                }
                else if(m.numJollies() == numJollies
                        && m.numToDelete() > numDel){
                    numDel = m.numToDelete();
                    chosen = i;
                }
            }
        }

        playMove(chosen, verbose);
    }

    //estraggo un numero random tra 0 e 1 e scelgo se fare la mossa più intelligente in base al qi del giocatore
    void playFuzzyMove(double qi, bool verbose = false){
        double r = rng.unifReal();
        if(r < qi)
            playSmartestMove(verbose);
        else
            playRandomMove(verbose);
    }
};

#endif // PLAYER_H
