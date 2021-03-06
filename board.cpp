#include "board.h"

Board::Board(int seed)
    {
        m_randcol = new RandomColor(this);
        m_randcol->setSeed(seed);
    }

CellMask& Board::rMask(const Point& point){
    return m_mask.rCell(point);
}

CellMask Board::mask(const Point& point) const{
    return m_mask.cell(point);
}

void Board::startNewGame(const GameParams * gameParams){
    m_size = gameParams->boardSize();
    m_numColors = gameParams->colorCount();
    m_diamonds.resize( m_size * m_size);
    for(auto& d : m_diamonds){
        d = 0;  //da ora in poi se il diamante è assente il puntatore sarà 0
    }
    m_mask.set(gameParams->mask());
    m_isDiamGenBiased = gameParams->isDiamGenBiased();
    m_biasDiamGen = gameParams->biasDiamGen();
    //scelgo il generatore di colori appropriato ai parametri
    m_randcol->init();
    spawnDiamonds();
}



void Board::spawnDiamonds(){
    clearSelection();
    clearDiamonds();
    for (Point point; point.x() < m_size; ++point.rx()){
        for (point.ry() = 0; point.y() < m_size; ++point.ry()){
            //displacement vectors needed for the following alsynagorithm
            //roll the dice to get a color, but ensure that there are not three of a color in a row from the start
            if(isOccupable(point)){ //potenzialmente la cella può contenere un diamante
                Color color;
                while (true){ //genera un colore finchè non si formano triplette
                    color = m_randcol->unif(); // genero un colore uniformemente a caso

                    const Point dispY1(0, -1), dispY2(0, -2);
                    const Point dispX1(-1, 0), dispX2(-2, 0);
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
	return diamond;
}

Diamond*& Board::rDiamond(const Point& point){
	return m_diamonds[point.x() + point.y() * m_size];
}

Diamond* Board::diamond(const Point& point) const{
    return m_diamonds[point.x() + point.y() * m_size];
}

int Board::gridSize() const{
	return m_size;
}

bool Board::isOccupable(const Point& point) const{
    bool isInBoard = 0 <= point.x() && point.x() < m_size && 0 <= point.y() && point.y() < m_size;
    return isInBoard && (mask(point) != CellMask::WALL) ;
}

bool Board::hasDiamond(const Point& point) const{
	return isOccupable(point) && diamond(point) != 0;
    //In teoria se tutto è stato fatto bene si potrebbe sostituire con il semplce
    //return  diamond(point) != 0;
}

vector<Point> Board::selections() const{
	return m_selections;
}

bool Board::hasSelection(const Point& point) const{
    return contains(m_selections, point);
}

void Board::setSelection(const Point& point, bool selected){
    const int index = indexOf(m_selections, point);
	if ((index >= 0) == selected)
		//nothing to do
		return;
	if (selected){
		//add selection, possibly by reusing an old item instance
		Diamond* selector;
        if (!m_inactiveSelectors.empty()){
            selector = m_inactiveSelectors.back();
            m_inactiveSelectors.pop_back();
        } else{
            selector = new Diamond(Color::Selection);
        }
        m_activeSelectors.push_back(selector);
        m_selections.push_back(point);
	}
	else{
		//remove selection, but try to reuse item instance later
        removeAt(m_selections, index);
        Diamond* selector = takeAt(m_activeSelectors, index);
        m_inactiveSelectors.push_back(selector);
	}
}

void Board::clearSelection(){
    for(Diamond* selector : m_activeSelectors){
        m_inactiveSelectors.push_back(selector);
	}
	m_selections.clear();
	m_activeSelectors.clear();
}

void Board::removeDiamond(const Point& point){
	Diamond* diamond = this->diamond(point);
	//può capitare che la funzione possa essere chiamate più volte nello stesso punto
	//se il diamante appartiene a più figure da scoppiare
    if(diamond != 0) {
        delete diamond;
        rDiamond(point) = 0;
	}
    if(mask(point) == CellMask::GELATINA){
        breakGelatina(point);
    } else if(mask(point) == CellMask::LIQUIRIZIA){
        breakLiquirizia(point);
    }
}


//il controllo se c´e´ una gelatina viene effettuato a monte
void Board::breakGelatina(const Point& point){
    rMask(point) = CellMask::BLANK;
}

//il controllo se c´e´ una gelatina viene effettuato a monte
void Board::breakLiquirizia(const Point& point){
    rMask(point) = CellMask::BLANK;
}

void Board::swapDiamonds(const Point& point1, const Point& point2){
    //Non posso swappare se c'è una liquirizia sotto
    if(mask(point1) == CellMask::LIQUIRIZIA || mask(point2) == CellMask::LIQUIRIZIA ) return;
	//swap diamonds in internal representation
	Diamond* diamond1 = this->diamond(point1);
	Diamond* diamond2 = this->diamond(point2);
	rDiamond(point1) = diamond2;
	rDiamond(point2) = diamond1;
}

void Board::fillGaps(){
    dropDiamonds();
    generateFromAbove();
}

void Board::generateFromAbove(){
    //fill top rows with new elements
    for (int x = 0; x < m_size; ++x){
        for (int y = m_size - 1; y >= 0; --y){
            Diamond*& diamond = this->rDiamond(Point(x, y));
            if (diamond || mask(Point(x, y)) == CellMask::WALL)
                continue; //inside of diamond stack - no gaps to fill
            auto color = m_isDiamGenBiased ? m_randcol->biased(x, m_biasDiamGen) : m_randcol->unif();
            diamond = spawnDiamond(color);
        }
    }
}

//fa cadere i diamanti esistenti
void Board::dropDiamonds(){
    for (int x = 0; x < m_size; ++x){
        // We have to search from the bottom of the column.
        // Exclude the lowest element (x = m_size - 1) because it cannot move down.
        for (int y = m_size - 2; y >= 0; --y){
            if (!diamond(Point(x, y))) //c'è un gap o un WALL
                // no need to move gaps -> these are moved later
                continue;
            // I muri devono essere attraversati dai diamanti
            int ys = y + 1; // conterrà la posizione di dove finisce il muro
            while(ys <  m_size && mask(Point(x, ys)) == CellMask::WALL){
                ++ys;
            }
            if(ys == m_size) // il muro arriva fino al fondo e blocca il diamante
                continue;
            if (diamond(Point(x, ys)))
                // there is something right below this diamond -> Do not move.
                continue;
            // search for the lowest possible position
            int yt = ys; //counters - (x, yt) is the target position of diamond (x,y)
            while(yt < m_size - 1){
                ys = yt + 1;
                while(ys <  m_size && mask(Point(x, ys)) == CellMask::WALL){
                    ++ys;
                }
                if (ys == m_size) //sono arrivato al muro di fondo
                    break; // yt now holds the lowest possible position
                if (diamond(Point(x, ys)))
                    break; // yt now holds the lowest possible position
                yt = ys;
            }
            rDiamond(Point(x, yt)) = diamond(Point(x, y));
            rDiamond(Point(x, y)) = 0;
            // if this element is selected, move the selection, too
            const int index = indexOf(m_selections, Point(x, y));
            if (index != -1){
                m_selections[index] =  Point(x, yt);
            }
        }
    }
}


void Board::clearDiamonds(){
    for (Point point; point.x() < m_size; point.rx()++)
		for (point.ry() = 0; point.y() < m_size; point.ry()++)
            if(hasDiamond(point)){
                removeDiamond(point);
            }
}

int Board::count(CellMask cell) const{
    int sum = 0;
    for (Point point; point.x() < m_size; point.rx()++)
        for (point.ry() = 0; point.y() < m_size; point.ry()++)
            if(mask(point) == cell)
                   sum++;
    return sum;
}

Board& Board::operator=(const Board& b){
    m_size = b.m_size;
    m_mask = b.m_mask;
    m_numColors = b.m_numColors;
    m_verbose = b.m_verbose;
    m_isDiamGenBiased = b.m_isDiamGenBiased;
    m_biasDiamGen = b.m_biasDiamGen;

    //ATTENZIONE, assumo che i diamanti esistano gia´ e siano nelle stesse posizioni
    for (Point point; point.x() < m_size; ++point.rx()){
        for (point.ry() = 0; point.y() < m_size; ++point.ry()){
            if(b.hasDiamond(point)){
                if(!this->hasDiamond(point)){
                    rDiamond(point) = spawnDiamond();
                }
                *diamond(point) = *(b.diamond(point));
            }
            if(!b.hasDiamond(point) && this->hasDiamond(point)){
                removeDiamond(point);
            }

        }
    }

    return *this;
}


