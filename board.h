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

class RandomColor{
public:
    RandomColor(int numColors)
        : m_numColors(numColors){}

    virtual void setSeed(int seed){
        m_rng.setSeed(seed);
    }

    virtual void copyStateRNG(const RandomColor * randcol){
        m_rng = randcol->m_rng;
    }

    virtual Color gen(){
        return Color(1 + m_rng.unifInt(m_numColors));
    }


private:
    LinCongRNG m_rng;
    int m_numColors;
};

class BiasedRandomColor : public RandomColor {
public:
    BiasedRandomColor(int numColors, double bias)
        : RandomColor(numColors)
        , m_bias(bias){}

//private:
    double m_bias;
};

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
    Diamond* spawnDiamond(Color color, JollyType jtype = JollyType::None);

    void clearDiamonds();

    int m_size;
    int m_numColors;
    int m_numDiamonds;
    QList<QPoint> m_selections;
    QVector<Diamond*> m_diamonds;
    QList<Diamond*> m_activeSelectors;
    QList<Diamond*>  m_inactiveSelectors;
    QVector<CellMask> m_mask;
    QVector<double> m_colorCount;
    RandomColor * m_randcol;

//DEBUG FUNCTONS
    void print() const{
        for(QPoint point; point.y() < m_size; point.ry()++){
            for(point.rx() = 0; point.x() < m_size; point.rx()++){
                if(hasDiamond(point)){
                    auto d = diamond(point);
                    
                    // questo è come stampo ora:
                    if(d->isJolly()){
                        {
                            if (d->jollyType() == JollyType::H) cout << "H" << int(d->color());
                            if (d->jollyType() == JollyType::V) cout << "V" << int(d->color());
                            if (d->jollyType() == JollyType::Bag) cout << "B" << int(d->color());
                            if (d->jollyType() == JollyType::Cookie) cout << "*" << int(d->color());
                        }
                        cout << " ";
                    }
                    else cout << int(d->color()) << "  ";
                    
                    // questo è come vorrei stampare
                    /*
                    if(d->isJolly()){
                        //cout << "*";
                        
                         {
                         if (d->jollyType() == JollyType::H) cout << "H";
                         if (d->jollyType() == JollyType::V) cout << "V";
                         if (d->jollyType() == JollyType::Bag) cout << "B  ";
                         if (d->jollyType() == JollyType::Cookie) cout << "*  ";
                         }
                        
                         if (d->jollyType() == JollyType::H || d->jollyType() == JollyType::V) cout << int(d->color()) << " ";
                        continue;
                    }
                    cout << int(d->color()) << "  ";
                    */
                } else {
                    cout << "-  ";
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
