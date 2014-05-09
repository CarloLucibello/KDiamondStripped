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
    bool isDiamGenBiased() const;
    double biasDiamGen() const;
    void setLevel(int level);    // from 0 to 4

private:
    //i parametri predefiniti per i vari livelli
    const vector<int> boardSizes = {8, 12, 10, 8, 8}; //il primo 'e per test
    const vector<int> numColors = {6, 5, 5, 5, 6};
    const vector<int> numMoves = {40, 30, 20, 10, 10};
    const vector<int> pointsToReach = {70, 30, 20, 10, 10};
    const vector<bool> isDiamondGeneratorBiased = {true, false, false, false, false};
    const vector<double> biasDiamondGenerator = {0.1, 0, 0, 0, 0};

    // i parametri attuali
    int m_colors;
    int m_size;
    int m_level;
    int m_moves;
    int m_points;
    bool m_isDiamGenBiased;
    double m_biasDiamGen;

public:
    void print(){
        cout << "level \t" << m_level <<  endl;
        cout << "colors \t" << m_colors <<  endl;
        cout << "size \t" << m_size <<  endl;
        cout << "points \t" << m_points <<  endl;
        cout << "moves \t" << m_moves <<  endl;
    }
};

#endif // GAMEPARAMS_H
