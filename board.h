#ifndef BOARD_H
#define BOARD_H

#include "randomcolor.h"
#include "diamond.h"
#include "gameparams.h"
#include "mask.h"
#include "point.h"
#include "utilities.h"

#include <vector>
#include <iostream>  //DEBUG
using namespace std;

class Board {
public:
    Board(int seed = -1);
    void startNewGame(const GameParams* gameParams);

    void setMask(int level);
    CellMask mask(const Point& point) const;
    CellMask& rMask(const Point& point);

    int gridSize() const;
    Diamond* diamond(const Point& point) const;

    bool isOccupable(const Point& point) const;
    bool hasDiamond(const Point& point) const;

    vector<Point> selections() const;
    bool hasSelection(const Point& point) const;
    void setSelection(const Point& point, bool selected);
    void clearSelection();

    void removeDiamond(const Point& point);
    void breakGelatina(const Point& point);
    void breakLiquirizia(const Point& point);
    void swapDiamonds(const Point& point1, const Point& point2);
    void generateFromAbove();
    void dropDiamonds();
    void fillGaps();
    int count(CellMask cell) const;

    void clicked(const Point& point);
    void dragged(const Point& point, const Point& direction);
//private://SLOTS
    void slotAnimationFinished();
    void slotClicked();
    void slotDragged(const Point& direction);
//private:
    Diamond*& rDiamond(const Point& point);
    Diamond* spawnDiamond(Color color = Color::NoColor, JollyType jtype = JollyType::None);
    void spawnDiamonds();


    void clearDiamonds();

    Board& operator=(const Board& b); //ATTENZIONE aggiornare se si aggiungono nuovi membri
    int m_size;
    int m_numColors;
    vector<Point> m_selections;
    vector<Diamond*> m_diamonds;
    vector<Diamond*> m_activeSelectors;
    vector<Diamond*>  m_inactiveSelectors;
    Mask m_mask;
    RandomColor * m_randcol;
    bool m_verbose;
    bool m_isDiamGenBiased;
    double m_biasDiamGen;

//DEBUG FUNCTONS
    void print() const{
        for(Point point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){

                //stampa il diamante
                if(hasDiamond(point)){
                    diamond(point)->print();
                } else {
                    cout << "-";
                }

                //stampa la topologia
                auto cell = mask(point);
                if(cell == CellMask::WALL){
                    cout << "-";
                }
                else if(cell == CellMask::GELATINA){
                    cout << "G";
                }
                else if(cell == CellMask::LIQUIRIZIA){
                    cout << "L";
                } else{ //caso CellMask::Blank
                    cout << "";
                }

                //lascio uno spazio
                cout << "\t";
            }
            cout << endl;
            cout << endl;
        }
    }

    void printMask() const{
        for(Point point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){
                cout << int(mask(point)) << " ";
            }
            cout << endl;
        }
    }

    void printSelection(const vector<Point>& points) const{
        for(Point point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){
                if(hasDiamond(point) && contains(points, point)){
                    diamond(point)->print();
                } else {
                    cout << "-   ";
                }

                //lascio uno spazio
                cout << "\t";
            }
            cout << endl;
            cout << endl;
        }
    }
};


#endif // BOARD_H
