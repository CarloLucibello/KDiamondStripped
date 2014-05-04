#include "game.h"

#include <cassert>
using namespace std;

Game::Game(int seed)
	: m_gameState(new GameState)
	, m_gameParams(new GameParams)
	, m_board(new Board(seed)){}

Game::~Game(){
    delete m_board;
	delete m_gameState;
	delete m_gameParams;
}

void Game::startNewGame(){
    m_gameState->startNewGame(m_gameParams);
    m_board->setParams(m_gameParams);
    m_board->startNewGame();
    m_jobQueue << Job::UpdateAvailableMovesJob;
    executeJobs();
}

//Funzione attualmente inutile
void Game::setMode(const Mode mode){
    m_gameState->setMode(mode);
}

void Game::setLevel(const int level){
    m_gameParams->setLevel(level);
}

//Checks amount of possible moves remaining
void Game::getMoves(){
	m_availableMoves.clear();
	const int gridSize = m_board->gridSize();
	for (QPoint point; point.x() < gridSize - 1; ++point.rx()){
		for (point.ry() = 0; point.y() < gridSize - 1; ++point.ry()){
			if(!m_board->hasDiamond(point)) continue;
             //guardo solo a sinistra e in basso, gli altri casi sono considerati
             //dagli altri punti
            QVector<QPoint> destinations = {point + QPoint(1, 0), point + QPoint(0, 1)};
            for(auto dest : destinations){
                if(m_board->hasDiamond(dest)){
                    m_board->swapDiamonds(point, dest); //ATTENZIONE questo swap non deve produrre animazioni
                    auto rH1 = findFigureRowH(point);
                    auto rH2 = findFigureRowH(dest);
                    auto rV1 = findFigureRowV(point);
                    auto rV2 = findFigureRowV(dest);
                    if(rH1.size() > 2 || rH2.size() > 2
                        || rV1.size() > 2 || rV2.size() > 2){
                        m_availableMoves.append({point, dest});
                    }
                    m_board->swapDiamonds(point, dest);
                }
            }
        }
    }

    if (m_availableMoves.isEmpty()){
		m_board->clearSelection();
		m_gameState->setState(State::Finished); //TODO, forse va agginto un EndGameJob
	}
}

//ritorna la riga verticale contenente il punto
QVector<QPoint> Game::findFigureRowV(const QPoint& point){
    QVector<QPoint> row;
    #define C(X, Y) (m_board->hasDiamond(QPoint(X, Y)) ? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
    Color currColor = m_board->diamond(point)->color();  //ATTENZIONE Non controllo che il colore sia valido (!=Selection)
    int yt = point.y();
    const int x = point.x();
    while(C(x, yt) == currColor){ //ciclo verso il basso
        row.append(QPoint(x, yt));
        yt++;
    }

    yt = point.y() - 1;
    while(C(x, yt) == currColor){ //ciclo verso l'alto
        row.append(QPoint(x, yt));
        yt--;
    }
    #undef C
    return row;
}

//ritorna la riga orizzontale contenente il punto
QVector<QPoint> Game::findFigureRowH(const QPoint& point){
    QVector<QPoint> row;
    #define C(X, Y) (m_board->hasDiamond(QPoint(X, Y)) ? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
    Color currColor = m_board->diamond(point)->color();  //ATTENZIONE Non controllo che il colore sia valido (!=Selection)
    int xt = point.x();
    const int y = point.y();
    while(C(xt, y) == currColor){ //ciclo verso destra
        row.append(QPoint(xt, y));
        xt++;
    }

    xt = point.x() - 1;
    while(C(xt, y) == currColor){ //ciclo verso sinistra
        row.append(QPoint(xt, y));
        xt--;
    }
    #undef C
    return row;
}


void Game::clickDiamond(const QPoint& point){
	if (m_gameState->state() != State::Playing)
		return;
	//do not allow more than two selections
	const bool isSelected = m_board->hasSelection(point);
	if (!isSelected && m_board->selections().count() == 2)
		return;
	//select only adjacent diamonds (i.e. if a distant diamond is selected, deselect the first one)
	foreach(const QPoint& point2, m_board->selections()){
		const int diff = qAbs(point2.x() - point.x()) + qAbs(point2.y() - point.y());
		if (diff > 1)
			m_board->setSelection(point2, false);
	}
	//toggle selection state
	m_board->setSelection(point, !isSelected);
	if (m_board->selections().count() == 2){
		m_jobQueue << Job::SwapDiamondsJob;
		executeJobs();
    }
}

void Game::dragDiamond(const QPoint& point, const QPoint& direction){
	//direction must not be null, and must point along one axis
	if ((direction.x() == 0) ^ (direction.y() == 0)){
		//find target indices
		const QPoint point2 = point + direction;
		if(!m_board->hasDiamond(point2))
			return;
		//simulate the clicks involved in this operation
		m_board->clearSelection();
		m_board->setSelection(point, true);
		m_board->setSelection(point2, true);
		m_jobQueue << Job::SwapDiamondsJob;
	}
}

bool Game::executeFirstJob(){
	if(m_jobQueue.isEmpty()){
		return false;
	}
	//execute next job in queue
	const Job job = m_jobQueue.takeFirst();
	switch (job){
		case Job::SwapDiamondsJob: {
		cout <<"Job::SwapDiamondsJob" << endl;
			assert(m_board->selections().count() == 2);
			const QList<QPoint> points = m_board->selections();
			m_swappingDiamonds = points;
            m_board->swapDiamonds(points[0], points[1]);
			m_jobQueue << Job::RemoveRowsJob; //We already insert this here to avoid another conditional statement.
            break;
		} //fall through

		case Job::RevokeSwapDiamondsJob:
//		cout<<"Job::RevokeSwapDiamondsJob" << endl;
			//invoke movement
			m_board->swapDiamonds(m_swappingDiamonds[0], m_swappingDiamonds[1]);
			break;

		case Job::RemoveRowsJob: {
//				cout<<"Job::RemoveRowJob" << endl;
			//find diamond rows and delete these diamonds
			const QList<QPoint> diamondsToRemove = findCompletedRows();
//							cout<<"Job::RemoveRowJob1" << endl;
//            cout<<"Diamons to remove " << diamondsToRemove.size() << endl;
			if (diamondsToRemove.isEmpty()){
				//no diamond rows were formed by the last move -> revoke movement (unless we are in a cascade)
				if (!m_swappingDiamonds.isEmpty()){
					m_jobQueue.prepend(Job::RevokeSwapDiamondsJob);
				}
				else {
                    if(m_gameState->movesLeft() == 0){
                        m_jobQueue << Job::EndGameJob;
                    } else {
                        m_jobQueue << Job::UpdateAvailableMovesJob;
                    }
                }
			}
			else{ //C'è qualcosa da rimuovere

				//all moves may now be out-dated - flush the moves list
				if (!m_availableMoves.isEmpty()){
					m_availableMoves.clear();
				}
				//Controllo se sto swappando e dato che lo swap ha avuto successo
				// incremento il numero di mosse ed elimino la selezione
				if(!m_swappingDiamonds.isEmpty()){
                    m_gameState->updateMovesLeft();
                    m_swappingDiamonds.clear();
                    m_board->clearSelection();
				}
				//
//				//report to Game
				m_gameState->addPoints(diamondsToRemove.size());
				//invoke remove animation, then fill gaps immediately after the animation
				foreach (const QPoint& diamondPos, diamondsToRemove)
					m_board->removeDiamond(diamondPos);
				m_jobQueue.prepend(Job::FillGapsJob);
//				printBoard();
			}
			break;
		}

		case Job::FillGapsJob:
//		cout << "Job::FillGapsJob:" << endl;
			//fill gaps
			m_board->fillGaps();
			m_jobQueue.prepend(Job::RemoveRowsJob); //allow cascades (i.e. clear rows that have been formed by falling diamonds)
//			printBoard();
			break;

		case Job::UpdateAvailableMovesJob:
//		cout <<"Job::UpdateAvailableMovesJob:" << endl;
			if (m_gameState->state() != State::Finished)
				getMoves();
			break;

		case Job::EndGameJob:
//		cout <<"Job::EndGameJob" << endl;
            m_gameState->setState(State::Finished);
			break;
	}

	return true;
}

void Game::executeJobs(){
    bool nonEmptyList = true;
    while(nonEmptyList){
        nonEmptyList = executeFirstJob();
    }
}

QList<QPoint> Game::findCompletedRows(){
	//The tactic of this function is brute-force. For now, I do not have a better idea
	Color currentColor;
	QList<QPoint> diamonds;
	int x, y, xh, yh; //counters
	const int gridSize = m_board->gridSize();
#define C(X, Y) (m_board->hasDiamond(QPoint(X, Y))? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
	//searching in horizontal direction
	for (y = 0; y < gridSize; ++y){
		for (x = 0; x < gridSize - 2; ++x){ //counter stops at gridSize - 2 to ensure availability of indices x + 1, x + 2
			currentColor = C(x, y);
			if(currentColor == Color::Selection)
                continue;
			if (currentColor != C(x + 1, y))
				continue;
			if (currentColor != C(x + 2, y))
				continue;
			//If the execution is here, we have found a row of three diamonds starting at (x,y).
			diamonds << QPoint(x, y);
			diamonds << QPoint(x + 1, y);
			diamonds << QPoint(x + 2, y);
			//Does the row have even more elements?
			if (x + 3 >= gridSize){
				//impossible to locate more diamonds - do not go through the following loop
				x += 2;
				continue;
			}
			for (xh = x + 3; xh <= gridSize - 1; ++xh){
				if (currentColor == C(xh, y))
					diamonds << QPoint(xh, y);
				else
					break; //row has stopped before this diamond - no need to continue searching
			}
			x = xh - 1; //do not search at this position in the row anymore (add -1 because x is incremented before the next loop)
		}
	}
	//searching in vertical direction (essentially the same algorithm, just with swapped indices -> no comments here, read the comments above)
	for (x = 0; x < gridSize; ++x){
		for (y = 0; y < gridSize - 2; ++y){
			currentColor = C(x, y);
            if(currentColor == Color::Selection)
                continue;
			if (currentColor != C(x, y + 1))
				continue;
			if (currentColor != C(x, y + 2))
				continue;
			diamonds << QPoint(x, y);
			diamonds << QPoint(x, y + 1);
			diamonds << QPoint(x, y + 2);
			if (y + 3 >= gridSize){
				y += 2;
				continue;
			}
			for (yh = y + 3; yh <= gridSize - 1; ++yh){
				if (currentColor == C(x, yh))
					diamonds << QPoint(x, yh);
				else
					break;
			}
			y = yh - 1;
		}
	}
#undef C
	return diamonds;
}

const QVector<Move>& Game::availMoves() const{
    return m_availableMoves;
}

bool Game::isFinished() const{
    return State::Finished == m_gameState->state();
}

bool Game::isWon() const {
    return m_gameState->points() >= m_gameParams->points();
}

int Game::points() const {
    return m_gameState->points();
}
