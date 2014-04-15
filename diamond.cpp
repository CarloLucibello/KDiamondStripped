#include "diamond.h"

Diamond::Diamond(Color color) : m_color(color){}

Color Diamond::color() const{
	return m_color;
}
