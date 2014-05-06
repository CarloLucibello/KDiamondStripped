#include "diamond.h"

Diamond::Diamond(Color color)
    : m_color(color)
    , m_isJollyH(false)
    , m_isJollyV(false)
    , m_isCookie(false){}

Color Diamond::color() const{
	return m_color;
}
