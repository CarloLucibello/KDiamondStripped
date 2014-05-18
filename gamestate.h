#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gameparams.h"
#include <iostream>
using namespace std;

enum class Mode{
    NormalGame,
    UntimedGame
};

enum class State{
    Playing,
    Finished
};

class GameState{
    public:
        GameState();
        Mode mode() const;
        int points() const;
        State state() const;

        void setState(State state);
        void setMode(Mode mode);
        void updateMovesLeft(int n = 1);
        int movesLeft();
        void addPoints(int removedDiamonds);
        void removePoints(int points);
        void startNewGame(const GameParams * gameParams);
//    private:
        Mode m_mode;
        State m_state;
        int m_points;
        int m_movesLeft;

        void print(){
            cout << "state \t" << int(m_state) <<  endl;
            cout << "points \t" << points() <<  endl;
            cout << "movesLeft \t" << movesLeft() <<  endl;
        }
};

#endif // GAMESTATE_H
