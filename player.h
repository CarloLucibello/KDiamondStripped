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

    void playRandomMove(){
        const auto& moves = m_game->availMoves();
        auto m = moves[rng.unifInt(moves.size())];
        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);
        cout << "MOVING : " << m.first.x() << " " << m.first.y() << " --> " <<m.second.x() << " " << m.second.y() << endl;
    }
    
    void playRandomMove_smarter(double qi){
        const auto& moves = m_game->availMoves();
        
        vector<int> count;
        count.resize(moves.size());
        
        //per ogni mossa, valuto quante caramelle scoppio
        //a seconda della direzione della mossa, cerco caramelle uguali a qualla
        //che sto spostando lungo la direzione parallela alla mossa
        //e lungo la direzione trasversa.
        //in quest'ultimo caso cerco in entrambi i versi. nel primo caso
        //devo cercare soltanto lungo il verso conconde allo spostamento.
        
        for (int i=0; i<moves.size(); ++i){
            
            auto m = moves[i];
            
            QPoint point(m.first.x(),m.first.y());
            QPoint dir(m.second.x()-m.first.x(), m.second.y()-m.first.y());
            
            int size=m_game->m_board->m_size;
            
            int n=2;
            int count_par=1;
            QPoint point1=point+n*dir;
            while ((point1.x()<size && point1.x()>=0) && (point1.y()<size && point1.y()>=0)){
                //cout << "point1: " << point1.x() << " " << point1.y() << endl;
                if (m_game->m_board->diamond(point1)->color()==m_game->m_board->diamond(point)->color())
                    count_par++;
                else break;
                n++;
                point1=point+n*dir;
                
            }
            
            int count_tra=1;
            QPoint dir_tra(dir.y(), dir.x());
            
            n=1;
            point1=point+dir+n*dir_tra;
            while ((point1.x()<size && point1.x()>=0) && (point1.y()<size && point1.y()>=0)){
                //cout << "point1: " << point1.x() << " " << point1.y() << endl;
                if (m_game->m_board->diamond(point1)->color()==m_game->m_board->diamond(point)->color())
                    count_tra++;
                else break;
                n++;
                point1=point+dir+n*dir_tra;
            }
            
            n=-1;
            point1=point+dir+n*dir_tra;
            while ((point1.x()<size && point1.x()>=0) && (point1.y()<size && point1.y()>=0)){
                //mcout << "point1: " << point1.x() << " " << point1.y() << endl;
                if (m_game->m_board->diamond(point1)->color()==m_game->m_board->diamond(point)->color())
                    count_tra++;
                else break;
                n--;
                point1=point+dir+n*dir_tra;
                
            }
            
            /*
             cout << "*****************" << endl;
             cout << "i= " << i << " " << "count_par=" << count_par << " " << "count_tra=" << count_tra << endl;
             cout << "point: " << point.x() << " " << point.y() << " dir: " << dir.x() << " " << dir.y() << endl;
             */
            count_tra > count_par ? count[i]=count_tra : count[i]=count_par;
            
        }
        
        //cerco la mossa più conveniente da fare:
        int argmax=0;
        for (int i=1; i<moves.size(); ++i) if (count[i] > count[argmax]) argmax=i;
        
        //estraggo un numero random tra 0 e 1 e scelgo se fare la mossa più intelligente in base al qi del giocatore
        
        LinCongRNG rng1;
        rng1.setSeed(-1);
        //double r=double(rng1.unifInt())/ 0xFFFFFFFFFFFFFFFFULL;
        
        double r=double(rng1.unifInt())/ 9223372036854775807;
        cout << "r= " << r << endl;
        int mossa_scelta;
        if (r<=qi)
            mossa_scelta=argmax;
        else
            mossa_scelta=rng.unifInt(moves.size());
        
        auto m = moves[mossa_scelta];
        
        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);
        cout << "MOVING :  " << m.first.x() << "  " << m.first.y() << " --> " << m.second.x() << "  " << m.second.y() <<  endl;
    }

    

};

#endif // PLAYER_H
