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
    RemoveFiguresJob, //remove complete rows of diamonds and add points
    RevokeSwapDiamondsJob, //revoke swapping of diamonds (will be requested by the RemoveRowsJob if no rows have been formed)
    FillGapsJob,
    UpdateAvailableMovesJob, //find and count available moves after the board has been changed
    EndGameJob //announce end of game
};

enum class FigureType {
    None,
    RowH = 1,
    RowV,
    LT
};

class Figure{
public:

    FigureType m_type;
    QVector<QPoint> m_points;

    Figure() {}

    Figure(QVector<QPoint> points, FigureType type)
        : m_points(points)
        , m_type(type) {}

    QVector<QPoint> points() const{
        return m_points;
    }

    FigureType type() const {
        return m_type;
    }

    int size() const {
        return m_points.size();
    }

    bool isEmpty() const {
        return m_points.isEmpty();
    }
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
    Game(int seed = -1, bool verbose = false);
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
    void removeDiamond(const QPoint& point);
    void removeJolly(const QPoint& point);

//  private:
//    QList<QPoint> findCompletedRows();
    QVector<Figure> findFigures();
    Figure findFigure(QPoint point);
    QVector<QPoint> findRowH(const QPoint& point);
    QVector<QPoint> findRowV(const QPoint& point);
    QVector<QPoint> findDiamonds(Color color);
    Figure findFigureCookie(QPoint p1, QPoint p2);

    void getMoves();
    const QVector<Move>& availMoves() const;

    QList<Job> m_jobQueue;
    QVector<Move> m_availableMoves;
    QList<QPoint> m_swappingDiamonds;

    Board* m_board;
    GameState* m_gameState;
    GameParams* m_gameParams;
    bool m_verbose;


//DEBUG FUNCTIONS
    void printBoard(){
        cout << "### BOARD ####" << endl;
        m_board->print();
        cout << endl;
    }

    void printSelection(const QVector<QPoint>& points){
        m_board->printSelection(points);
        cout << endl;
    }

    void printMoves(){
        cout << "### MOVES ####" << endl;
        for(auto& m : m_availableMoves){
            cout << m.from().x() << "  " << m.from().y() << " <--> " << m.to().x() << "  " << m.to().y() <<  endl;
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
