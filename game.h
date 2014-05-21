#ifndef GAME_H
#define GAME_H

#include <QPoint>
#include <QList>
#include "board.h"
#include "gamestate.h"
#include "gameparams.h"
#include <utility>
#include "figure.h"
using namespace std;

//jobs to be done during the board update
enum class Job {
    SwapDiamonds = 1, //swap selected diamonds
    RemoveFigures, //remove complete rows of diamonds and add points
    RevokeSwapDiamonds, //revoke swapping of diamonds (will be requested by the RemoveRowsJob if no rows have been formed)
    FillGaps,
    UpdateAvailableMoves, //find and count available moves after the board has been changed
    EndGame, //announce end of game
    NoMoves //respawn the diamons

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

    int points() const{
        return m_points;
    }

    int liquirizie() const{
        return m_liquirizie;
    }

    bool gelatine() const{
        return m_gelatine;
    }

private:
    QPoint m_from;
    QPoint m_to;
    int m_points;
    int m_liquirizie;
    int m_gelatine;
};

class Game{
    int Bag2attiva;
    //questa flag è -1 di default, e solo quando scoppio una busta è messa
    //uguale alla coordinata x della busta
    bool uploadingmoves;
    //questa flag si attiva solo quando sto aggiornando le mosse
public:
    Game(int seed = -1, bool verbose = false);
    ~Game();
    void startNewGame();
    void setMode(const Mode mode);
    void setLevel(const int level);
    void setParamsPath(string paramsPath);
    void clickDiamond(const QPoint& point);
    void dragDiamond(const QPoint& point, const QPoint& direction);
    void executeJobs();
    bool executeFirstJob();
    int points() const;
    bool isFinished() const;
    bool isWon() const;
    void removeDiamond(const QPoint& point);
    void removeJolly(const QPoint& point);
    void removeFigures(const QVector<Figure>& figuresToRemove);

    void getJollies(const Figure& fig, QVector<JollyType>& jtypes,
                   QVector<QPoint> excludedPoints = QVector<QPoint>()) const;


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

    Board * m_board;

    Board * m_testBoard; //per testare le mosse;
    GameState* m_testState;

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
