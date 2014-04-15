#include "game.h"

Game::Game()
    : m_board(0)
	, m_gameState(new GameState)
	, m_gameParams(new GameParams)
{
	m_jobQueue << Job::UpdateAvailableMovesJob;
}

Game::~Game(){
    delete m_board;
	delete m_gameState;
	delete m_gameParams;
}

void Game::startNewGame(){
    m_gameState->startNewGame();

    delete m_board; //si potrebbe riciclare la board per rendere la cosa più efficiente
    m_board = new Board(m_gameParams);
}


void Game::setMode(const Mode& mode){
    m_gameState->setMode(mode);
}

void Game::setLevel(const int& level){
    m_gameParams->setLevel(level);
}

//Checks amount of possible moves remaining
void Game::getMoves(){
#define C(X, Y) (m_board->hasDiamond(QPoint(X, Y)) ? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
	m_availableMoves.clear();
	Color curColor;
	const int gridSize = m_board->gridSize();
	for (int x = 0; x < gridSize; ++x){
		for (int y = 0; y < gridSize; ++y){
			curColor = C(x, y);
			if (curColor == C(x + 1, y)){
				if (curColor == C(x - 2, y))
					m_availableMoves.append(QPoint(x - 2, y));
				if (curColor == C(x - 1, y - 1))
					m_availableMoves.append(QPoint(x - 1, y - 1));
				if (curColor == C(x - 1, y + 1))
					m_availableMoves.append(QPoint(x - 1, y + 1));
				if (curColor == C(x + 3, y))
					m_availableMoves.append(QPoint(x + 3, y));
				if (curColor == C(x + 2, y - 1))
					m_availableMoves.append(QPoint(x + 2, y - 1));
				if (curColor == C(x + 2, y + 1))
					m_availableMoves.append(QPoint(x + 2, y + 1));
			}
			if (curColor == C(x + 2, y)){
				if (curColor == C(x + 1, y - 1))
					m_availableMoves.append(QPoint(x + 1, y - 1));
				if (curColor == C(x + 1, y + 1))
					m_availableMoves.append(QPoint(x + 1, y + 1));
			}
			if (curColor == C(x, y + 1)){
				if (curColor == C(x, y - 2))
					m_availableMoves.append(QPoint(x, y - 2));
				if (curColor == C(x - 1, y - 1))
					m_availableMoves.append(QPoint(x - 1, y - 1));
				if (curColor == C(x + 1, y - 1))
					m_availableMoves.append(QPoint(x + 1, y - 1));
				if (curColor == C(x, y + 3))
					m_availableMoves.append(QPoint(x + 3, y));
				if (curColor == C(x - 1, y + 2))
					m_availableMoves.append(QPoint(x - 1, y + 2));
				if (curColor == C(x + 1, y + 2))
					m_availableMoves.append(QPoint(x + 1, y + 2));
			}
			if (curColor == C(x, y + 2)){
				if (curColor == C(x - 1, y + 1))
					m_availableMoves.append(QPoint(x - 1, y + 1));
				if (curColor == C(x + 1, y + 1))
					m_availableMoves.append(QPoint(x + 1, y + 1));
			}
		}
	}
#undef C
	if (m_availableMoves.isEmpty()){
		m_board->clearSelection();
		m_gameState->setState(State::Finished);
	}
}

void Game::clickDiamond(const QPoint& point){
	if (m_gameState->state() != State::Playing)
		return;
	//do not allow more than two selections
	const bool isSelected = m_board->hasSelection(point);
	if (!isSelected && m_board->selections().count() == 2)
		return;
	//select only adjacent diamonds (i.e. if a distant diamond is selected, deselect the first one)
	foreach(const QPoint& point2, m_board->selections()){ //TODO: foreach è c++ standard o è QT?
		const int diff = qAbs(point2.x() - point.x()) + qAbs(point2.y() - point.y());
		if (diff > 1)
			m_board->setSelection(point2, false);
	}
	//toggle selection state
	m_board->setSelection(point, !isSelected);
	if (m_board->selections().count() == 2)
		m_jobQueue << Job::SwapDiamondsJob;
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
			if (m_board->selections().count() != 2)
				break; //this can be the case if, during a cascade, two diamonds are selected (inserts SwapDiamondsJob) and then deselected
			//ensure that the selected diamonds are neighbors (this is not necessarily the case as diamonds can move to fill gaps)
			const QList<QPoint> points = m_board->selections();
			m_board->clearSelection();
			const int dx = qAbs(points[0].x() - points[1].x());
			const int dy = qAbs(points[0].y() - points[1].y());
			if (dx + dy != 1)
				break;
			//start a new cascade
			m_gameState->resetCascadeCounter();
			//copy selection info into another storage (to allow the user to select the next two diamonds while the cascade runs)
			m_swappingDiamonds = points;
			m_jobQueue << Job::RemoveRowsJob; //We already insert this here to avoid another conditional statement.
		} //fall through
		case Job::RevokeSwapDiamondsJob:
			//invoke movement
			m_board->swapDiamonds(m_swappingDiamonds[0], m_swappingDiamonds[1]);
			break;
		case Job::RemoveRowsJob: {
			//find diamond rows and delete these diamonds
			const QList<QPoint> diamondsToRemove = findCompletedRows();
			if (diamondsToRemove.isEmpty()){
				//no diamond rows were formed by the last move -> revoke movement (unless we are in a cascade)
				if (!m_swappingDiamonds.isEmpty())
					m_jobQueue.prepend(Job::RevokeSwapDiamondsJob);
				else
					m_jobQueue << Job::UpdateAvailableMovesJob;
			}
			else{
				//all moves may now be out-dated - flush the moves list
				if (!m_availableMoves.isEmpty())
				{
					m_availableMoves.clear();
			//ATTENZIONE		emit numberMoves(-1);
				}
				//it is now safe to delete the position of the swapping diamonds
				m_swappingDiamonds.clear();
				//report to Game
				m_gameState->addPoints(diamondsToRemove.count());
				//invoke remove animation, then fill gaps immediately after the animation
				foreach (const QPoint& diamondPos, diamondsToRemove)
					m_board->removeDiamond(diamondPos);
				m_jobQueue.prepend(Job::FillGapsJob);
			}
			break;
		}
		case Job::FillGapsJob:
			//fill gaps
			m_board->fillGaps();
			m_jobQueue.prepend(Job::RemoveRowsJob); //allow cascades (i.e. clear rows that have been formed by falling diamonds)
			break;
		case Job::UpdateAvailableMovesJob:
			if (m_gameState->state() != State::Finished)
				getMoves();
			break;
		case Job::EndGameJob:
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
	//The tactic of this function is brute-force. For now, I do not have a better idea: A friend of mine advised me to look in the environment of moved diamonds, but this is not easy since the re-filling after a deletion can lead to rows that are far away from the original movement. Therefore, we simply search through all diamonds looking for combinations in the horizonal and vertical direction.
	Color currentColor;
	QList<QPoint> diamonds;
	int x, y, xh, yh; //counters
	const int gridSize = m_board->gridSize();
#define C(X, Y) m_board->diamond(QPoint(X, Y))->color()
	//searching in horizontal direction
	for (y = 0; y < gridSize; ++y){
		for (x = 0; x < gridSize - 2; ++x){ //counter stops at gridSize - 2 to ensure availability of indices x + 1, x + 2
			currentColor = C(x, y);
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



