#ifndef BOARD_H
#define BOARD_H

#include "diamond.h"
#include "gameparams.h"
#include "rand.h"

#include <QVector>

#include <vector>
#include <iostream>  //DEBUG
using namespace std;

class Board {
public:
    LinCongRNG rng;

    Board(GameParams* gameParams, int seed = -1);

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
    QList<Diamond*> m_activeSelectors, m_inactiveSelectors;

//DEBUG FUNCTONS
    void print() const{
        for(int y = 0; y < m_size; y++){
            for(int x = 0; x < m_size; x++){
                cout << int(diamond({x, y})->color()) << " ";
            }
            cout << endl;
        }
    }
};


#endif // BOARD_H
