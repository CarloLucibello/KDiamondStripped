#ifndef BOARD_H
#define BOARD_H

#include "diamond.h"
#include "gameparams.h"
#include "rand.h"

#include <QVector>

#include <vector>
#include <iostream>  //DEBUG
using namespace std;

enum class CellMask {
    BLANK,  //normal cell
    WALL
};

class Board {
public:
    LinCongRNG rng;
    Board();
    void setParams(const GameParams* gameParams);
    void startNewGame(int seed = -1);

    void setMask(int level);
    CellMask mask(const QPoint& point) const;
    CellMask& rMask(const QPoint& point);

    int gridSize() const;
    Diamond* diamond(const QPoint& point) const;

    bool hasDiamond(const QPoint& point) const;
    QList<QPoint> selections() const;
    bool hasSelection(const QPoint& point) const;
    void setSelection(const QPoint& point, bool selected);
    void clearSelection();

    void removeDiamond(const QPoint& point);
    void swapDiamonds(const QPoint& point1, const QPoint& point2);
    void fillGaps();

    void clicked(const QPoint& point);
    void dragged(const QPoint& point, const QPoint& direction);
//private://SLOTS
    void slotAnimationFinished();
    void slotClicked();
    void slotDragged(const QPoint& direction);
//private:
    QPoint findDiamond(Diamond* diamond) const;
    Diamond*& rDiamond(const QPoint& point);
    Diamond* spawnDiamond(Color color);

    int m_size;
    int m_colorCount;
    QList<QPoint> m_selections;
    QVector<Diamond*> m_diamonds;
    QList<Diamond*> m_activeSelectors;
    QList<Diamond*>  m_inactiveSelectors;
    QVector<CellMask> m_mask;

//DEBUG FUNCTONS
    void print() const{
        for(QPoint point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){
                if(hasDiamond(point)){
                    cout << int(diamond(point)->color()) << " ";
                } else {
                    cout << "- ";
                }
            }
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
};


#endif // BOARD_H
