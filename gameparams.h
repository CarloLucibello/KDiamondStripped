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
    string m_paramsPath = "levels/params.txt";
    // questo è il file che contiene tutti i parametri che si possono modificare.
    // per ora contiene 19 livelli tutti uguali, a parte la maschera.
    
    GameParams();
    virtual ~GameParams();
    int colorCount() const;
    int boardSize() const;
    int moves() const;
    int level() const;    // per ora questo è un valore che va da 1 a 19
                          // vedi il file levels/params.
    int points() const;
    int mask() const;
    
    bool isDiamGenBiased() const;
    double biasDiamGen() const;
    bool targetLiquirizia() const;
    void setParamsPath(string paramsPath);
    void setLevel(int level);
    
private:
    // i parametri attuali
    int m_colors;
    int m_mask;
    int m_level;
    int m_moves;
    int m_points;
    bool m_isDiamGenBiased;
    double m_biasDiamGen;
    bool m_targetLiquirizia;
    const int m_size = 9;
    //m_size è il lato della matrice con cui giochiamo.
    //per ora lo fissiamo a 9 perchè è dato dalla maschera.
    //TODO per ora la fisso a 9, in futuro sara´ data dalla mask

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
