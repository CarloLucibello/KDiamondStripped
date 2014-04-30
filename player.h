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
            cout << "MOVING : " << m.first.x() << " " << m.first.y() << " --> " << m.second.x() << " " << m.second.y() << endl;
            cout << endl;
        }
        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);

   }

    void playSmartRandomMove(double qi, bool verbose = false){
        const auto& moves = m_game->availMoves();

        vector<int> count;
        count.resize(moves.size());

        //per ogni mossa, valuto quante caramelle scoppio
        //a seconda della direzione della mossa, cerco caramelle uguali a qualla
        //che sto spostando lungo la direzione parallela alla mossa
        //e lungo la direzione trasversa.
        //in quest'ultimo caso cerco in entrambi i versi. nel primo caso
        //devo cercare soltanto lungo il verso conconde allo spostamento.

        for (int i = 0; i < moves.size(); ++i){
            auto m = moves[i];

            QPoint point(m.first.x(), m.first.y());
            QPoint dir(m.second.x() - m.first.x(), m.second.y() - m.first.y());

            int size = m_game->m_board->m_size;
            auto board = m_game->m_board;
            int n = 2; //parte da due perché viene spostato
            int count_par = 1;
            QPoint point1 = point + n * dir;
            auto currColor = board->diamond(point)->color();
            while (board->hasDiamond(point1)){
                //cout << "point1: " << point1.x() << " " << point1.y() << endl;
                if (board->diamond(point1)->color() == currColor){
                    count_par++;
                    n++;
                    point1 =  point + n * dir;
                }
                else
                    break;
            }

            int count_tra=1;
            QPoint dir_tra(dir.y(), dir.x());

            n = 1;
            point1 = point + dir + n * dir_tra;
            while (board->hasDiamond(point1)){
                //cout << "point1: " << point1.x() << " " << point1.y() << endl;
                if (board->diamond(point1)->color() == currColor){
                    count_tra++;
                    n++;
                    point1 = point + dir + n * dir_tra;
                }
                else
                    break;
            }

            n = -1;
            point1 = point + dir + n * dir_tra;
            while (board->hasDiamond(point1)){
                //mcout << "point1: " << point1.x() << " " << point1.y() << endl;
                if (board->diamond(point1)->color() == currColor)
                    count_tra++;
                else break;
                n--;
                point1 = point + dir + n * dir_tra;

            }
            /*
             cout << "*****************" << endl;
             cout << "i= " << i << " " << "count_par=" << count_par << " " << "count_tra=" << count_tra << endl;
             cout << "point: " << point.x() << " " << point.y() << " dir: " << dir.x() << " " << dir.y() << endl;
             */
            if(count_tra > count_par)
                count[i] = count_tra;
            else
                count[i] = count_par;
        }

        //cerco la mossa più conveniente da fare:
        int argmax = 0;
        for (int i = 1; i < moves.size(); ++i)
            if (count[i] > count[argmax]) argmax = i;

        //estraggo un numero random tra 0 e 1 e scelgo se fare la mossa più intelligente in base al qi del giocatore
        double r = rng.unifReal();
        int mossa_scelta = r < qi ? argmax : rng.unifInt(moves.size());
        playMove(mossa_scelta, verbose);
    }



};

#endif // PLAYER_H
