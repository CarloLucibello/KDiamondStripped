#ifndef BOARD_H
#define BOARD_H

#include "randomcolor.h"
#include "diamond.h"
#include "gameparams.h"
#include "mask.h"

#include <QVector>

#include <vector>
#include <iostream>  //DEBUG
using namespace std;

class Board {
public:
    Board(int seed);
    void setParams(const GameParams* gameParams);
    void startNewGame();

    void setMask(int level);
    CellMask mask(const QPoint& point) const;
    CellMask& rMask(const QPoint& point);

    int gridSize() const;
    Diamond* diamond(const QPoint& point) const;

    bool isOccupable(const QPoint& point) const;
    bool hasDiamond(const QPoint& point) const;

    QList<QPoint> selections() const;
    bool hasSelection(const QPoint& point) const;
    void setSelection(const QPoint& point, bool selected);
    void clearSelection();

    void removeDiamond(const QPoint& point);
    void breakGelatina(const QPoint& point);
    void breakLiquirizia(const QPoint& point);
    void swapDiamonds(const QPoint& point1, const QPoint& point2);
    void fillGaps();
    int count(CellMask cell) const;

    void clicked(const QPoint& point);
    void dragged(const QPoint& point, const QPoint& direction);
//private://SLOTS
    void slotAnimationFinished();
    void slotClicked();
    void slotDragged(const QPoint& direction);
//private:
    QPoint findDiamond(Diamond* diamond) const;
    Diamond*& rDiamond(const QPoint& point);
    Diamond* spawnDiamond(Color color, JollyType jtype = JollyType::None);

    void clearDiamonds();

    int m_size;
    int m_numColors;
    int m_numDiamonds;
    QList<QPoint> m_selections;
    QVector<Diamond*> m_diamonds;
    QList<Diamond*> m_activeSelectors;
    QList<Diamond*>  m_inactiveSelectors;
    Mask m_mask;
    QVector<double> m_colorCount;
    RandomColor * m_randcol;
    bool m_verbose;
    bool m_isDiamGenBiased;
    double m_biasDiamGen;

//DEBUG FUNCTONS
    void print() const{
        for(QPoint point; point.y() < m_size; point.ry()++){
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
        for(QPoint point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){
                cout << int(mask(point)) << " ";
            }
            cout << endl;
        }
    }

    void printSelection(const QVector<QPoint>& points) const{
        for(QPoint point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){
                if(hasDiamond(point) && points.contains(point)){
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
