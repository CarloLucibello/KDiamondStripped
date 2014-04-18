#include "board.h"

Board::Board(GameParams * gameParams, int seed)
	: m_size(gameParams->boardSize())
	, m_colorCount(gameParams->colorCount())
	, m_diamonds(m_size * m_size, 0)
{
    rng.setSeed(seed);
	for (QPoint point; point.x() < m_size; ++point.rx())
		for (point.ry() = 0; point.y() < m_size; ++point.ry()){
			//displacement vectors needed for the following alsynagorithm
			const QPoint dispY1(0, -1), dispY2(0, -2);
			const QPoint dispX1(-1, 0), dispX2(-2, 0);
			//roll the dice to get a color, but ensure that there are not three of a color in a row from the start
			Color color;
			while (true){

                color = Color(1+rng.unifInt(m_colorCount));//+1 because numbering of enum KDiamond::Color starts at 1
                //color = Color(qrand() % m_colorCount + 1); // +1 because numbering of enum KDiamond::Color starts at 1
				//condition: no triplet in y axis (attention: only the diamonds above us are defined already)
				if (point.y() >= 2){ //no triplet possible for i = 0, 1
					const Color otherColor1 = diamond(point + dispY1)->color();
					const Color otherColor2 = diamond(point + dispY2)->color();
					if (otherColor1 == color && otherColor2 == color)
						continue; //roll the dice again
				}
				//same condition on x axis
				if (point.x() >= 2){
					const Color otherColor1 = diamond(point + dispX1)->color();
					const Color otherColor2 = diamond(point + dispX2)->color();
					if (otherColor1 == color && otherColor2 == color)
						continue;
				}
				break;
			}
			rDiamond(point) = spawnDiamond(color);
		}
}

Diamond* Board::spawnDiamond(Color color){
	Diamond* diamond = new Diamond(color);
	return diamond;
}

QPoint Board::findDiamond(Diamond* diamond) const{
	int index = m_diamonds.indexOf(diamond);
	if (index == -1)
		return QPoint(-1, -1);
	else
		return QPoint(index % m_size, index / m_size);
}

Diamond*& Board::rDiamond(const QPoint& point){
	return m_diamonds[point.x() + point.y() * m_size];
}

Diamond* Board::diamond(const QPoint& point) const
{
	return m_diamonds.value(point.x() + point.y() * m_size);
}

int Board::gridSize() const{
	return m_size;
}

bool Board::hasDiamond(const QPoint& point) const{
	return 0 <= point.x() && point.x() < m_size && 0 <= point.y() && point.y() < m_size;
}

QList<QPoint> Board::selections() const{
	return m_selections;
}

bool Board::hasSelection(const QPoint& point) const{
	return m_selections.contains(point);
}

void Board::setSelection(const QPoint& point, bool selected){
	const int index = m_selections.indexOf(point);
	if ((index >= 0) == selected)
		//nothing to do
		return;
	if (selected){
		//add selection, possibly by reusing an old item instance
		Diamond* selector;
		if (!m_inactiveSelectors.isEmpty())
			selector = m_inactiveSelectors.takeLast();
		else
			selector = new Diamond(Color::Selection);
		m_activeSelectors << selector;
		m_selections << point;
	}
	else{
		//remove selection, but try to reuse item instance later
		m_selections.removeAt(index);
		Diamond* selector = m_activeSelectors.takeAt(index);
		m_inactiveSelectors << selector;
	}
}

void Board::clearSelection(){
	foreach (Diamond* selector, m_activeSelectors){
		m_inactiveSelectors << selector;
	}
	m_selections.clear();
	m_activeSelectors.clear();
}

//mi sembra uno spreco fare delete
void Board::removeDiamond(const QPoint& point){
	Diamond* diamond = this->diamond(point);
	delete diamond;//diamond has already been removed
	rDiamond(point) = 0;
}


void Board::swapDiamonds(const QPoint& point1, const QPoint& point2){
	//swap diamonds in internal representation
	Diamond* diamond1 = this->diamond(point1);
	Diamond* diamond2 = this->diamond(point2);
	rDiamond(point1) = diamond2;
	rDiamond(point2) = diamond1;
}

void Board::fillGaps(){
	//fill gaps
	int x, y, yt; //counters - (x, yt) is the target position of diamond (x,y)
	for (x = 0; x < m_size; ++x){
		//We have to search from the bottom of the column. Exclude the lowest element (x = m_size - 1) because it cannot move down.
		for (y = m_size - 2; y >= 0; --y){
			if (!diamond(QPoint(x, y)))
				//no need to move gaps -> these are moved later
				continue;
			if (diamond(QPoint(x, y + 1)))
				//there is something right below this diamond -> Do not move.
				continue;
			//search for the lowest possible position
			for (yt = y; yt < m_size - 1; ++yt){
				if (diamond(QPoint(x, yt + 1)))
					break; //xt now holds the lowest possible position
			}
			rDiamond(QPoint(x, yt)) = diamond(QPoint(x, y));
			rDiamond(QPoint(x, y)) = 0;
			//if this element is selected, move the selection, too
			const int index = m_selections.indexOf(QPoint(x, y));
			if (index != -1){
				m_selections.replace(index, QPoint(x, yt));
			}
		}
	}
	//fill top rows with new elements
	for (x = 0; x < m_size; ++x){
		yt = 0; //now: holds the position from where the diamond comes (-1 for the lowest new diamond)
		for (y = m_size - 1; y >= 0; --y){
			Diamond*& diamond = this->rDiamond(QPoint(x, y));
			if (diamond)
				continue; //inside of diamond stack - no gaps to fill
			--yt;
			diamond = spawnDiamond(Color(rng.unifInt(m_colorCount) + 1));
		}
	}
}


