#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "rand.h"



class Player{

    vector<int> count;
    vector<int> count_p;
    vector<int> count_t;
    
    QList<QPoint> directions;
    
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
        
        m_game->mossaSelezionata = m;
        
        if (m_game->m_board->diamond(m.first)->color() == Color::JollyScoppiaTutto){
            m_game->m_board->usoJollySpaccaTutto=1;
            cout << "USO IL JOLLY SPACCA TUTTO" << endl;
        }
        
        if (count[i]==4){
            
            m_game->m_board->creaJollyHorV=1;
            //questo è il punto in cui devo andare a creare il jolly
            m_game->m_board->pointJolly=m.second;
            if (abs(directions[i].x()) > abs(directions[i].y())){
                if (count_p[i]>count_t[i])
                {
                    cout << "CREO CARAMELLA JOLLY ORIZZONTALE" << endl;
                    m_game->m_board->hOrvFlagJolly=0;
                }
                else
                {
                    m_game->m_board->hOrvFlagJolly=1;
                    cout << "CREO CARAMELLA JOLLY VERTICALE" << endl;
                }
            }
            else
                if (count_p[i]>count_t[i])
                {
                    cout << "CREO CARAMELLA JOLLY VERTICALE" << endl;
                    m_game->m_board->hOrvFlagJolly=1;
                    
                }
                else
                {
                    cout << "CREO CARAMELLA JOLLY ORIZZONTALE" << endl;
                    m_game->m_board->hOrvFlagJolly=0;
                }
        }
        
        if (count[i]==5){
            m_game->m_board->creaJollySpaccaTutto=1;
            m_game->m_board->pointJolly=m.second;
            cout << "CREO CARAMELLA JOLLY SCOPPIA TUTTO" << endl;
        }

        
        m_game->clickDiamond(m.first);
        m_game->clickDiamond(m.second);

        
        count.resize(0);
        count_p.resize(0);
        count_t.resize(0);
        directions.clear();
      
   }

    void playSmartRandomMove(double qi, bool verbose = false){
        m_game->m_board->creaJollyHorV=0;
        m_game->m_board->creaJollySpaccaTutto=0;
        
        const auto& moves = m_game->availMoves();

        const auto& movesJ_spaccatutto = m_game->availMovesJ_spaccatutto();
        const auto& movesJ_H = m_game->availMovesJ_H();
        const auto& movesJ_V = m_game->availMovesJ_V();
        
        
        
        /*
         
        // se c'è un jolly spacca tutto lo uso 
        if (movesJ_spaccatutto.size()>0){
            
            int i_mossa=rng.unifInt(4*movesJ_spaccatutto.size());
            auto m = moves[i_mossa];
            
            QPoint point(m.first.x(), m.first.y());
            QPoint dir(m.second.x() - m.first.x(), m.second.y() - m.first.y());
            
            directions.append(dir);
            
        }
        
        */
         
        count.resize(moves.size());
        count_p.resize(moves.size());
        count_t.resize(moves.size());
        
        //per ogni mossa, valuto quante caramelle scoppio
        //a seconda della direzione della mossa, cerco caramelle uguali a quella
        //che sto spostando lungo la direzione parallela alla mossa
        //e lungo la direzione trasversa.
        //in quest'ultimo caso cerco in entrambi i versi. nel primo caso
        //devo cercare soltanto lungo il verso concorde allo spostamento.


        
        for (int i = 0; i < moves.size(); ++i){
            auto m = moves[i];

            QPoint point(m.first.x(), m.first.y());
            QPoint dir(m.second.x() - m.first.x(), m.second.y() - m.first.y());

            directions.append(dir);
            
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
            
            count_t[i]=count_tra;
            count_p[i]=count_par;
            
            
        }

        
        //cerco la mossa più conveniente da fare:
        int argmax = 0;
        for (int i = 1; i < moves.size(); ++i)
            if (count[i] > count[argmax]) argmax = i;

        //estraggo un numero random tra 0 e 1 e scelgo se fare la mossa più intelligente in base al qi del giocatore
        double r = rng.unifReal();
        int mossa_scelta = r < qi ? argmax : rng.unifInt(moves.size());
        playMove(mossa_scelta, verbose);
        
        if (verbose){
            cout << "count_tra" << " count_par" << endl;
            cout << count_t[mossa_scelta] << " " << count_p[mossa_scelta] << endl;
        }
        
    }



};

#endif // PLAYER_H
