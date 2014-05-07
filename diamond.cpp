#include "diamond.h"

Diamond::Diamond(Color color, JollyType jollyType)
    : m_color(color)
    , m_jollyType(jollyType){}

Color Diamond::color() const{
	return m_color;
}

bool Diamond::isJolly() const{
    return m_jollyType != JollyType::None;
}

void Diamond::setJolly(JollyType type){
    m_jollyType = type;
}

JollyType Diamond::jollyType() const{
    return m_jollyType;
}
