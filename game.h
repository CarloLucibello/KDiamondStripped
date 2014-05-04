#ifndef GAME_H
#define GAME_H

#include <QPoint>
#include <QList>
#include "board.h"
#include "gamestate.h"
#include "gameparams.h"

#include <utility>
using namespace std;




//jobs to be done during the board update
enum class Job {
    SwapDiamondsJob = 1, //swap selected diamonds
    RemoveRowsJob, //remove complete rows of diamonds and add points
    RevokeSwapDiamondsJob, //revoke swapping of diamonds (will be requested by the RemoveRowsJob if no rows have been formed)
    FillGapsJob,
    UpdateAvailableMovesJob, //find and count available moves after the board has been changed
    EndGameJob //announce end of game
};

class Move{
    friend class Game;
public:
    Move(){}

    Move(const QPoint& from, const QPoint& to)
        : m_from(from)
        , m_to(to){}

    QPoint from() const{
        return m_from;
    }

    QPoint to() const {
        return m_to;
    }

    const QVector<QPoint>& toDelete(){
        return m_toDelete;
    }

    int numToDelete() const{
        return m_toDelete.size();
    }


private:
    QPoint m_from;
    QPoint m_to;
    QVector<QPoint> m_toDelete;
};


class Game{
public:
    Game(int seed);
    ~Game();
    void startNewGame();
    void setMode(const Mode mode);
    void setLevel(const int level);
    void clickDiamond(const QPoint& point);
    void dragDiamond(const QPoint& point, const QPoint& direction);
    void executeJobs();
    bool executeFirstJob();
    int points() const;
    bool isFinished() const;
    bool isWon() const;
//  private:
    QList<QPoint> findCompletedRows();
    QVector<QPoint> findFigureRowH(const QPoint& point);
    QVector<QPoint> findFigureRowV(const QPoint& point);

    void getMoves();
    const QVector<Move>& availMoves() const;

    QList<Job> m_jobQueue;
    QVector<Move> m_availableMoves;
    QList<QPoint> m_swappingDiamonds;
    Board* m_board;
    GameState* m_gameState;
    GameParams* m_gameParams;


//DEBUG FUNCTIONS
    void printBoard(){
        cout << "### BOARD ####" << endl;
        m_board->print();
        cout << endl;
    }

    void printMoves(){
        cout << "### MOVES ####" << endl;
        for(auto& m : m_availableMoves){
            cout << m.from().x() << "  " << m.from().y() << " --> " << m.to().x() << "  " << m.to().y() <<  endl;
        }
        cout << endl;
    }

    void printState(){
        cout << "### STATE ####" << endl;
        m_gameState->print();
        cout << endl;
    }

    void printParams(){
        cout << "### PARAMS ####" << endl;
        m_gameParams->print();
        cout << endl;
    }

};


#endif // GAME_H
