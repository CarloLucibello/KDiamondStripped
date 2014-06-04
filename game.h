#ifndef GAME_H
#define GAME_H


#include <vector>
#include <utility>
#include <deque>

using namespace std;
#include "board.h"
#include "gamestate.h"
#include "gameparams.h"
#include "figure.h"
#include "point.h"


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

    Move(const Point& from, const Point& to)
        : m_from(from)
        , m_to(to){}

    Point from() const{
        return m_from;
    }

    Point to() const {
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
    Point m_from;
    Point m_to;
    int m_points;
    int m_liquirizie;
    int m_gelatine;
};

class Game{
public:
    Game(int seed = -1, bool verbose = false);
    ~Game();
    void startNewGame();
    void setMode(const Mode mode);
    void setLevel(const int level);
    void setParamsPath(string paramsPath);
    void clickDiamond(const Point& point);
    void dragDiamond(const Point& point, const Point& direction);
    void executeJobs();
    bool executeFirstJob();
    int points() const;
    bool isFinished() const;
    bool isWon() const;
    void removeDiamond(const Point& point);
    void removeJolly(const Point& point);
    void removeFigures(const vector<Figure>& figuresToRemove);

    void getJollies(const Figure& fig, vector<JollyType>& jtypes,
                   vector<Point> excludedPoints = vector<Point>()) const;


//  private:
//  vector<Point> findCompletedRows();
    vector<Figure> findFigures();
    Figure findFigure(Point point);
    vector<Point> findRowH(const Point& point);
    vector<Point> findRowV(const Point& point);
    vector<Point> findDiamonds(Color color);
    Figure findFigureCookie(Point p1, Point p2);

    void getMoves();

    const vector<Move>& availMoves() const;

    deque<Job> m_jobQueue;
    vector<Move> m_availableMoves;
    vector<Point> m_swappingDiamonds;

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

    void printSelection(const vector<Point>& points){
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
