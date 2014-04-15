#ifndef GAME_H
#define GAME_H

#include <QPoint>
#include <QList>
#include "board.h"
#include "gamestate.h"
#include "gameparams.h"




//jobs to be done during the board update
enum class Job {
    SwapDiamondsJob = 1, //swap selected diamonds
    RemoveRowsJob, //remove complete rows of diamonds and add points
    RevokeSwapDiamondsJob, //revoke swapping of diamonds (will be requested by the RemoveRowsJob if no rows have been formed)
    FillGapsJob,
    UpdateAvailableMovesJob, //find and count available moves after the board has been changed
    EndGameJob //announce end of game
};

class Game{
public:
    Game();
    ~Game();
    void startNewGame();
    void setMode(const Mode& mode);
    void setLevel(const int& level);
    void clickDiamond(const QPoint& point);
    void dragDiamond(const QPoint& point, const QPoint& direction);
    void numberMoves(int moves);
    void executeJobs();
    bool executeFirstJob();
//  private:
    QList<QPoint> findCompletedRows();
    void getMoves();

    QList<Job> m_jobQueue;
    QList<QPoint> m_availableMoves;
    QList<QPoint> m_swappingDiamonds;
    Board* m_board;
    GameState* m_gameState;
    GameParams* m_gameParams;


//DEBUG FUNCTIONS
    void printBoard(){
        m_board->print();
    }


};


#endif // GAME_H
