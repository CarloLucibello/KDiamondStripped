#ifndef GAMESTATE_H
#define GAMESTATE_H


enum class Mode{
    NormalGame,
    UntimedGame
};

enum class State{
    Playing,
    Finished
};

class GameStatePrivate{
    public:
        GameStatePrivate();
        ~GameStatePrivate();

        Mode m_mode;
        State m_state;
        int m_points;
};

class GameState{
    public:
        GameState();
        ~GameState();
        Mode mode() const;
        int points() const;
        State state() const;

        void setState(State state);
        void setMode(Mode mode);
        void addPoints(int removedDiamonds);
        void removePoints(int points);
        void startNewGame();
//    private:
        GameStatePrivate* p;
};

#endif // GAMESTATE_H
