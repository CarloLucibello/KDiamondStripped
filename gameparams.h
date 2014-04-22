#ifndef GAMEPARAMS_H
#define GAMEPARAMS_H

#include <vector>
#include <iostream>
using namespace std;

/** La classe che contiene i parametri che definiscono il livello */
class GameParams{
public:
    GameParams();
    virtual ~GameParams();
    int colorCount() const;
    int boardSize() const;
    int moves() const;
    int level() const;    // from 0 to 4
    int points() const;    // from 0 to 4
    void setLevel(const int level);    // from 0 to 4

private:
    const vector<int> boardSizes = {9, 12, 10, 8, 8, 8 }; //il primo 'e per test
    const vector<int> boardColorCounts = {5, 5, 5, 5, 6, 7};
    const vector<int> numMoves = {10, 30, 20, 10, 10, 10};
    const vector<int> pointsToReach = {80, 30, 20, 10, 10, 10};

    int m_colorCount;
    int m_size;
    int m_level;
    int m_moves;
    int m_points;

public:
    void print(){
        cout << "**PARAMS******" << endl;
        cout << "level \t" << m_level <<  endl;
        cout << "colors \t" << m_colorCount <<  endl;
        cout << "size \t" << m_size <<  endl;
        cout << "points \t" << m_points <<  endl;
        cout << "moves \t" << m_moves <<  endl;
    }
};

#endif // GAMEPARAMS_H
