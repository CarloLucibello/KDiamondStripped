#ifndef DIAMOND_H
#define DIAMOND_H

#include <QPoint>
//registered colors of diamonds
enum class Color{
    NoColor = -1,  //use this if no actual color can be named (e.g. for a null Diamond pointer)
    Selection = 0, //actually no diamond type, but this allows to reuse the Diamond class' code for the selection marker
    RedDiamond = 1,
    GreenDiamond,  //2
    BlueDiamond,   //3
    YellowDiamond, //4
    WhiteDiamond,  //5
    BlackDiamond,  //6
    OrangeDiamond, //7
    ColorsCount, //questo colore non è usato in nessun livello. Al massimo si usano 7 colori
    //di seguito ci sono le caramelle jolly orizzontali e verticali
    JollyRed_H,    //9
    JollyGreen_H,  //10
    JollyBlue_H,   //11
    JollyYellow_H, //12
    JollyWhite_H,  //13
    JollyBlack_H,  //14
    JollyOrange_H, //15
    JollyRed_V,    //16
    JollyGreen_V,  //17
    JollyBlue_V,   //18
    JollyYellow_V, //19
    JollyWhite_V,  //20
    JollyBlack_V,  //21
    JollyOrange_V,  //22
    JollyScoppiaTutto //questa è la caramella che si forma facendo esplodere 5 caramelle di fila, e che fa scppiare tutte le caramelle del colore con cui faccio lo switch
};

class Diamond{
public:
    Diamond(Color color);

    Color color() const;
    void clicked();
    void dragged(const QPoint& direction);
private:
    Color m_color;
};


#endif // DIAMOND_H
