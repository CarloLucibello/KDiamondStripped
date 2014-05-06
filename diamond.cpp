#include "diamond.h"

Diamond::Diamond(Color color)
    : m_color(color)
    , m_isJollyH(false)
    , m_isJollyV(false)
    , m_isCookie(false){}

Color Diamond::color() const{
	return m_color;
}

bool Diamond::isJollyH() const{
    return m_isJollyH;
}

bool Diamond::isJollyV() const{
    return m_isJollyV;
}

bool Diamond::isCookie() const{
    return m_isCookie;
}

void Diamond::setJollyH(bool val){
    m_isJollyH = val;
}

void Diamond::setJollyV(bool val){
    m_isJollyV = val;
}

void Diamond::setCookie(bool val){
    m_isCookie = val;
}
