#include "board.h"

Board::Board(int seed)
    : m_numDiamonds(0)
    {
        m_randcol = new RandomColor(0); //tanto non verrà mai usato
        m_randcol->setSeed(seed);
    }

void Board::setParams(const GameParams * gameParams){
	m_size = gameParams->boardSize();
	m_numColors = gameParams->colorCount();
	m_colorCount.fill(0, m_numColors + 1); // i colori cominciano da 1
	m_diamonds.fill(0, m_size * m_size);
	setMask(gameParams->level());

	//Scelgo il generatore di colori appropriato ai parametri
	RandomColor * randcol;
	if(gameParams->isDiamGenBiased()){
        randcol = new BiasedRandomColor(m_numColors, gameParams->biasDiamGen());
	} else {
        randcol = new RandomColor(m_numColors);
	}
	//Aggiorno m_randcol copiando lo stato del vecchio generatore
	randcol->copyStateRNG(m_randcol);
	delete m_randcol;
	m_randcol = randcol;
}

//bisognerebbe includere la maschera in GameParams
void Board::setMask(int level){
    m_mask.fill(CellMask::BLANK, m_size * m_size);
    switch(level){
        case 0: // toglie i quattro angoli
            rMask({0, 0}) = CellMask::WALL;
            rMask({0, m_size - 1}) = CellMask::WALL;
            rMask({m_size - 1, 0}) = CellMask::WALL;
            rMask({m_size - 1, m_size - 1}) = CellMask::WALL;
            break;
    }
}

CellMask& Board::rMask(const QPoint& point){
	return m_mask[point.x() + point.y() * m_size];
}

CellMask Board::mask(const QPoint& point) const{
	return m_mask[point.x() + point.y() * m_size];
}

void Board::startNewGame(){
    clearDiamonds();
    for (QPoint point; point.x() < m_size; ++point.rx()){
        for (point.ry() = 0; point.y() < m_size; ++point.ry()){
            //displacement vectors needed for the following alsynagorithm
            //roll the dice to get a color, but ensure that there are not three of a color in a row from the start
            if(isOccupable(point)){ //potenzialmente la cella può contenere un diamante
                Color color;
                while (true){ //genera un colore finchè non si formano triplette
                     color = m_randcol->gen();

                    const QPoint dispY1(0, -1), dispY2(0, -2);
                    const QPoint dispX1(-1, 0), dispX2(-2, 0);
                    //condition: no triplet in y axis (attention: only the diamonds above us are defined already)
                    if (hasDiamond(point + dispY1) && hasDiamond(point + dispY2)){ //controlla che non ci siano muri o che non si sia al bordo
                        const Color otherColor1 = diamond(point + dispY1)->color();
                        const Color otherColor2 = diamond(point + dispY2)->color();
                        if (otherColor1 == color && otherColor2 == color)
                            continue; //roll the dice again
                    }
                    //same condition on x axis
                    if (hasDiamond(point + dispX1) && hasDiamond(point + dispX2)){
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
    }
}

Diamond* Board::spawnDiamond(Color color, JollyType jtype){
	Diamond* diamond = new Diamond(color, jtype);
    m_numDiamonds++;
    m_colorCount[int(color)]++;
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

Diamond* Board::diamond(const QPoint& point) const{
	return m_diamonds.value(point.x() + point.y() * m_size);
}

int Board::gridSize() const{
	return m_size;
}

bool Board::isOccupable(const QPoint& point) const{
    bool isInBoard = 0 <= point.x() && point.x() < m_size && 0 <= point.y() && point.y() < m_size;
	return isInBoard && mask(point) == CellMask::BLANK;
}


bool Board::hasDiamond(const QPoint& point) const{
	return isOccupable(point) && diamond(point) != 0;
    //In teoria se tutto è stato fatto bene si potrebbe sostituire con il semplce
    // return  diamond(point) != 0;
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

void Board::removeDiamond(const QPoint& point){
	Diamond* diamond = this->diamond(point);
	//può capitare che la funzione possa essere chiamate più volte nello stesso punto
	// se il diamante appartiene a più figure da scoppiare
	if(diamond != 0) {
        m_colorCount[int(diamond->color())]--;
        delete diamond;
        rDiamond(point) = 0;
        m_numDiamonds--;
	}
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
	for (int x = 0; x < m_size; ++x){
//		We have to search from the bottom of the column. Exclude the lowest element (x = m_size - 1) because it cannot move down.
		for (int y = m_size - 2; y >= 0; --y){
			if (!diamond(QPoint(x, y))) //c'è un gap o un WALL
//				no need to move gaps -> these are moved later
				continue;
            // I muri devono essere attraversati dai diamanti
            int ys = y + 1; //conterrà la posizione di dove finisce il muro
            while(ys <  m_size && mask(QPoint(x, ys)) == CellMask::WALL){
                ++ys;
            }
            if(ys == m_size) //il muro arriva fino al fondo e blocca il diamante
                continue;
			if (diamond(QPoint(x, ys)))
//				there is something right below this diamond -> Do not move.
				continue;
//			search for the lowest possible position
        	int yt = ys; //counters - (x, yt) is the target position of diamond (x,y)
			while(yt < m_size - 1){
                ys = yt + 1;
                while(ys <  m_size && mask(QPoint(x, ys)) == CellMask::WALL){
                    ++ys;
                }
                if (ys == m_size) //sono arrivato al muro di fondo
                    break; //yt now holds the lowest possible position
				if (diamond(QPoint(x, ys)))
					break; //yt now holds the lowest possible position
                yt = ys;
			}
			rDiamond(QPoint(x, yt)) = diamond(QPoint(x, y));
			rDiamond(QPoint(x, y)) = 0;
//			if this element is selected, move the selection, too
			const int index = m_selections.indexOf(QPoint(x, y));
			if (index != -1){
				m_selections.replace(index, QPoint(x, yt));
			}
		}
	}
	//fill top rows with new elements
	for (int x = 0; x < m_size; ++x){
		for (int y = m_size - 1; y >= 0; --y){
			Diamond*& diamond = this->rDiamond(QPoint(x, y));
			if (diamond || mask(QPoint(x, y)) == CellMask::WALL)
				continue; //inside of diamond stack - no gaps to fill
			diamond = spawnDiamond(m_randcol->gen());

            cout << "creato diamante di colore: " << int(diamond->color()) << " in " << x<< " " << y << endl;

		}
	}
}

void Board::clearDiamonds(){
    for (QPoint point; point.x() < m_size; point.rx()++)
		for (point.ry() = 0; point.y() < m_size; point.ry()++)
            if(hasDiamond(point)){
                removeDiamond(point);
            }
}

