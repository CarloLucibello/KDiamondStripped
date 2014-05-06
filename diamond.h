#ifndef DIAMOND_H
#define DIAMOND_H

#include <QPoint>
//registered colors of diamonds
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

class Diamond{
public:
    Diamond(Color color);

    Color color() const;
    void clicked();
    void dragged(const QPoint& direction);
    bool isJollyH();
    bool isJollyV();
    bool isCookie();

private:
    Color m_color;
    bool m_isJollyV;
    bool m_isJollyH;
    bool m_isCookie;
};


#endif // DIAMOND_H
