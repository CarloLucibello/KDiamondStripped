#ifndef GAMEPARAMS_H
#define GAMEPARAMS_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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
    int mask() const;
    bool isDiamGenBiased() const;
    double biasDiamGen() const;
    void setLevel(int level);    // from 0 to 4

private:
    // i parametri attuali
    int m_colors;
    int m_mask;
    int m_level;
    int m_moves;
    int m_points;
    bool m_isDiamGenBiased;
    double m_biasDiamGen;
    bool m_targetGelatina;
    const int m_size = 9; //TODO per ora la fisso a 9, in futuro sara´ data dalla mask
    string m_paramsPath = "levels/params.txt";

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
