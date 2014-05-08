#ifndef DIAMOND_H
#define DIAMOND_H

#include <QPoint>
//registered colors of diamonds

#include <iostream>
using namespace std;

enum class Color{
    NoColor = -1,  //use this if no actual color can be named (e.g. for a null Diamond pointer)
    Selection = 0, //actually no diamond type, but this allows to reuse the Diamond class' code for the selection marker
    RedDiamond = 1,
    GreenDiamond,
    BlueDiamond,
    YellowDiamond,
    WhiteDiamond,
    BlackDiamond,
    OrangeDiamond,
    ColorsCount
};

enum class JollyType {None = 0, H, V, Cookie, Bag};

class Diamond{
public:
    Diamond(Color color, JollyType jollyType = JollyType::None);

    Color color() const;
    void clicked();
    void dragged(const QPoint& direction);
    bool isJolly() const;
    void setJolly(JollyType type);
    JollyType jollyType() const;

    void print(){
      // questo è come stampo ora:
        if(isJolly()){
            if (jollyType() == JollyType::H) cout << "H" << int(color());
            if (jollyType() == JollyType::V) cout << "V" << int(color());
            if (jollyType() == JollyType::Bag) cout << "B" << int(color());
            if (jollyType() == JollyType::Cookie) cout << "*" << int(color());
            cout << " ";
        }
        else cout << int(color()) << "  ";

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
    }

private:
    Color m_color;
    JollyType m_jollyType;
};



#endif // DIAMOND_H
