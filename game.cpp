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
#define C(X, Y) (m_board->hasDiamond(QPoint(X, Y)) ? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
	m_availableMoves.clear();
    m_availableMovesJ_spaccatutto.clear();
    m_availableMovesJ_H.clear();
    m_availableMovesJ_V.clear();
    
	Color curColor;
	const int gridSize = m_board->gridSize();
	for (int x = 0; x < gridSize; ++x){
		for (int y = 0; y < gridSize; ++y){
			curColor = C(x, y);
			if(curColor == Color::Selection) continue;

            //mosse standard
            
			if (curColor == C(x + 1, y)){
                if(C(x - 1, y) != Color::Selection){ //controlla che il diamante si possa posizionare
                    if (curColor == C(x - 2, y))
                        m_availableMoves.append({QPoint(x - 2, y), QPoint(x - 1, y)});
                    if (curColor == C(x - 1, y - 1))
                        m_availableMoves.append({QPoint(x - 1, y - 1), QPoint(x - 1, y)});
                    if (curColor == C(x - 1, y + 1))
                        m_availableMoves.append({QPoint(x - 1, y + 1), QPoint(x - 1, y)});
                }

                if(C(x + 2, y) != Color::Selection){
                    if (curColor == C(x + 3, y))
                        m_availableMoves.append({QPoint(x + 3, y), QPoint(x + 2, y)});
                    if (curColor == C(x + 2, y - 1))
                        m_availableMoves.append({QPoint(x + 2, y - 1), QPoint(x + 2, y)});
                    if (curColor == C(x + 2, y + 1))
                        m_availableMoves.append({QPoint(x + 2, y + 1), QPoint(x + 2, y)});
                }
			}

			if (curColor == C(x + 2, y) && C(x + 1, y) != Color::Selection){
				if (curColor == C(x + 1, y - 1))
					m_availableMoves.append({QPoint(x + 1, y - 1), QPoint(x + 1, y)});
				if (curColor == C(x + 1, y + 1))
					m_availableMoves.append({QPoint(x + 1, y + 1), QPoint(x + 1, y)});
			}

			if (curColor == C(x, y + 1)){
                if(C(x, y - 1) != Color::Selection){
                    if (curColor == C(x, y - 2))
                        m_availableMoves.append({QPoint(x, y - 2), QPoint(x, y - 1)});
                    if (curColor == C(x - 1, y - 1))
                        m_availableMoves.append({QPoint(x - 1, y - 1), QPoint(x, y - 1)});
                    if (curColor == C(x + 1, y - 1))
                        m_availableMoves.append({QPoint(x + 1, y - 1), QPoint(x, y - 1)});
                }
                if(C(x, y + 2) != Color::Selection){
                    if (curColor == C(x, y + 3))
                        m_availableMoves.append({QPoint(x, y + 3), QPoint(x, y + 2)});
                    if (curColor == C(x - 1, y + 2))
                        m_availableMoves.append({QPoint(x - 1, y + 2), QPoint(x, y + 2)});
                    if (curColor == C(x + 1, y + 2))
                        m_availableMoves.append({QPoint(x + 1, y + 2), QPoint(x, y + 2)});
                }
			}

			if (curColor == C(x, y + 2) && C(x, y + 1) != Color::Selection){
				if (curColor == C(x - 1, y + 1))
					m_availableMoves.append({QPoint(x - 1, y + 1), QPoint(x, y + 1)});
				if (curColor == C(x + 1, y + 1))
					m_availableMoves.append({QPoint(x + 1, y + 1), QPoint(x, y + 1)});
			}
            
            // mosse possibili grazie alla caramella scoppia tutto
            
            if (curColor == Color::JollyScoppiaTutto){
                if (C(x+1, y ) != Color::Selection) m_availableMovesJ_spaccatutto.append({QPoint(x , y ), QPoint(x+1, y)});
                if (C(x, y+1 ) != Color::Selection) m_availableMovesJ_spaccatutto.append({QPoint(x , y ), QPoint(x, y+1)});
                if (C(x-1, y ) != Color::Selection) m_availableMovesJ_spaccatutto.append({QPoint(x , y ), QPoint(x-1, y)});
                if (C(x, y-1 ) != Color::Selection) m_availableMovesJ_spaccatutto.append({QPoint(x , y ), QPoint(x, y-1)});
            }
            
            // mosse possibili grazie ai jolly H
            
            if (int(curColor) > 8 && int(curColor) < 16 ){
                
                //cerco in tutte e quattro le direzioni
                //---//
                
                if (int(curColor) == (int(C(x + 1, y)) + 8)){
                    if(C(x - 1, y) != Color::Selection){ //controlla che il diamante si possa posizionare
                        if (int(curColor) == (int(C(x - 2, y)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 2, y), QPoint(x - 1, y)});
                        if (int(curColor) == (int(C(x - 1, y - 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 1, y - 1), QPoint(x - 1, y)});
                        if (int(curColor) == (int(C(x - 1, y + 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 1, y + 1), QPoint(x - 1, y)});
                    }
                    
                    if(C(x + 2, y) != Color::Selection){
                        if (int(curColor) == (int(C(x + 3, y)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 3, y), QPoint(x + 2, y)});
                        if (int(curColor) == (int(C(x + 2, y - 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 2, y - 1), QPoint(x + 2, y)});
                        if (int(curColor) == (int(C(x + 2, y + 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 2, y + 1), QPoint(x + 2, y)});
                    }
                }
                
                if ((int(curColor) == (int(C(x + 2, y)) + 8)) && C(x + 1, y) != Color::Selection){
                    if (int(curColor) == (int(C(x + 1, y - 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x + 1, y - 1), QPoint(x + 1, y)});
                    if (int(curColor) == (int(C(x + 1, y + 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x + 1, y + 1), QPoint(x + 1, y)});
                }
                
                //---//
                
                if (int(curColor) == (int(C(x, y + 1)) + 8) ){
                    if(C(x, y - 1) != Color::Selection){
                        if (int(curColor) == (int(C(x, y - 2)) + 8))
                            m_availableMovesJ_H.append({QPoint(x, y - 2), QPoint(x, y - 1)});
                        if (int(curColor) == (int(C(x - 1, y - 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 1, y - 1), QPoint(x, y - 1)});
                        if (int(curColor) == (int(C(x + 1, y - 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 1, y - 1), QPoint(x, y - 1)});
                    }
                    if(C(x, y + 2) != Color::Selection){
                        if (int(curColor) == (int(C(x, y + 3)) + 8))
                            m_availableMovesJ_H.append({QPoint(x, y + 3), QPoint(x, y + 2)});
                        if (int(curColor) == (int(C(x - 1, y + 2)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 1, y + 2), QPoint(x, y + 2)});
                        if (int(curColor) == (int(C(x + 1, y + 2)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 1, y + 2), QPoint(x, y + 2)});
                    }
                }
                
                if (int(curColor) == (int(C(x, y + 2)) + 8) && C(x, y + 1) != Color::Selection){
                    if (int(curColor) == (int(C(x - 1, y + 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x - 1, y + 1), QPoint(x, y + 1)});
                    if (int(curColor) == (int(C(x + 1, y + 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x + 1, y + 1), QPoint(x, y + 1)});
                }
                
                //---//
                
                if (int(curColor) == (int(C(x - 1, y)) + 8)){
                    if(C(x + 1, y) != Color::Selection){ //controlla che il diamante si possa posizionare
                        if (int(curColor) == (int(C(x + 2, y)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 2, y), QPoint(x + 1, y)});
                        if (int(curColor) == (int(C(x - 1, y - 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 1, y - 1), QPoint(x + 1, y)});
                        if (int(curColor) == (int(C(x - 1, y + 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 1, y + 1), QPoint(x + 1, y)});
                    }
                    
                    if(C(x - 2, y) != Color::Selection){
                        if (int(curColor) == (int(C(x - 3, y)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 3, y), QPoint(x - 2, y)});
                        if (int(curColor) == (int(C(x + 2, y - 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 2, y - 1), QPoint(x - 2, y)});
                        if (int(curColor) == (int(C(x + 2, y + 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 2, y + 1), QPoint(x - 2, y)});
                    }
                }
                
                if ((int(curColor) == (int(C(x - 2, y)) + 8)) && C(x - 1, y) != Color::Selection){
                    if (int(curColor) == (int(C(x - 1, y - 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x - 1, y - 1), QPoint(x - 1, y)});
                    if (int(curColor) == (int(C(x + 1, y + 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x - 1, y + 1), QPoint(x - 1, y)});
                }
                
                //---//
                
                if (int(curColor) == (int(C(x, y - 1)) + 8) ){
                    if(C(x, y + 1) != Color::Selection){
                        if (int(curColor) == (int(C(x, y + 2)) + 8))
                            m_availableMovesJ_H.append({QPoint(x, y + 2), QPoint(x, y + 1)});
                        if (int(curColor) == (int(C(x - 1, y + 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 1, y + 1), QPoint(x, y + 1)});
                        if (int(curColor) == (int(C(x + 1, y + 1)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 1, y + 1), QPoint(x, y + 1)});
                    }
                    if(C(x, y - 2) != Color::Selection){
                        if (int(curColor) == (int(C(x, y - 3)) + 8))
                            m_availableMovesJ_H.append({QPoint(x, y - 3), QPoint(x, y - 2)});
                        if (int(curColor) == (int(C(x - 1, y - 2)) + 8))
                            m_availableMovesJ_H.append({QPoint(x - 1, y - 2), QPoint(x, y - 2)});
                        if (int(curColor) == (int(C(x + 1, y - 2)) + 8))
                            m_availableMovesJ_H.append({QPoint(x + 1, y - 2), QPoint(x, y - 2)});
                    }
                }
                
                if (int(curColor) == (int(C(x, y - 2)) + 8) && C(x, y - 1) != Color::Selection){
                    if (int(curColor) == (int(C(x - 1, y - 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x - 1, y - 1), QPoint(x, y - 1)});
                    if (int(curColor) == (int(C(x + 1, y - 1)) + 8))
                        m_availableMovesJ_H.append({QPoint(x + 1, y - 1), QPoint(x, y - 1)});
                }
                
            }
            
            // mosse possibili grazie ai jolly V
            
            
            if (int(curColor) > 15 && int(curColor) < 23 ){
                
                //cerco in tutte e quattro le direzioni
                //---//
                
                if (int(curColor) == (int(C(x + 1, y)) + 15)){
                    if(C(x - 1, y) != Color::Selection){ //controlla che il diamante si possa posizionare
                        if (int(curColor) == (int(C(x - 2, y)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 2, y), QPoint(x - 1, y)});
                        if (int(curColor) == (int(C(x - 1, y - 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 1, y - 1), QPoint(x - 1, y)});
                        if (int(curColor) == (int(C(x - 1, y + 1)) + 15 ))
                            m_availableMovesJ_V.append({QPoint(x - 1, y + 1), QPoint(x - 1, y)});
                    }
                    
                    if(C(x + 2, y) != Color::Selection){
                        if (int(curColor) == (int(C(x + 3, y)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 3, y), QPoint(x + 2, y)});
                        if (int(curColor) == (int(C(x + 2, y - 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 2, y - 1), QPoint(x + 2, y)});
                        if (int(curColor) == (int(C(x + 2, y + 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 2, y + 1), QPoint(x + 2, y)});
                    }
                }
                
                if ((int(curColor) == (int(C(x + 2, y)) + 15)) && C(x + 1, y) != Color::Selection){
                    if (int(curColor) == (int(C(x + 1, y - 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x + 1, y - 1), QPoint(x + 1, y)});
                    if (int(curColor) == (int(C(x + 1, y + 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x + 1, y + 1), QPoint(x + 1, y)});
                }
                
                //---//
                
                if (int(curColor) == (int(C(x, y + 1)) + 15) ){
                    if(C(x, y - 1) != Color::Selection){
                        if (int(curColor) == (int(C(x, y - 2)) + 15))
                            m_availableMovesJ_V.append({QPoint(x, y - 2), QPoint(x, y - 1)});
                        if (int(curColor) == (int(C(x - 1, y - 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 1, y - 1), QPoint(x, y - 1)});
                        if (int(curColor) == (int(C(x + 1, y - 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 1, y - 1), QPoint(x, y - 1)});
                    }
                    if(C(x, y + 2) != Color::Selection){
                        if (int(curColor) == (int(C(x, y + 3)) + 15))
                            m_availableMovesJ_V.append({QPoint(x, y + 3), QPoint(x, y + 2)});
                        if (int(curColor) == (int(C(x - 1, y + 2)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 1, y + 2), QPoint(x, y + 2)});
                        if (int(curColor) == (int(C(x + 1, y + 2)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 1, y + 2), QPoint(x, y + 2)});
                    }
                }
                
                if (int(curColor) == (int(C(x, y + 2)) + 15) && C(x, y + 1) != Color::Selection){
                    if (int(curColor) == (int(C(x - 1, y + 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x - 1, y + 1), QPoint(x, y + 1)});
                    if (int(curColor) == (int(C(x + 1, y + 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x + 1, y + 1), QPoint(x, y + 1)});
                }
                
                //---//
                
                if (int(curColor) == (int(C(x - 1, y)) + 15)){
                    if(C(x + 1, y) != Color::Selection){
                        if (int(curColor) == (int(C(x + 2, y)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 2, y), QPoint(x + 1, y)});
                        if (int(curColor) == (int(C(x - 1, y - 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 1, y - 1), QPoint(x + 1, y)});
                        if (int(curColor) == (int(C(x - 1, y + 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 1, y + 1), QPoint(x + 1, y)});
                    }
                    
                    if(C(x - 2, y) != Color::Selection){
                        if (int(curColor) == (int(C(x - 3, y)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 3, y), QPoint(x - 2, y)});
                        if (int(curColor) == (int(C(x + 2, y - 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 2, y - 1), QPoint(x - 2, y)});
                        if (int(curColor) == (int(C(x + 2, y + 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 2, y + 1), QPoint(x - 2, y)});
                    }
                }
                
                if ((int(curColor) == (int(C(x - 2, y)) + 15)) && C(x - 1, y) != Color::Selection){
                    if (int(curColor) == (int(C(x - 1, y - 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x - 1, y - 1), QPoint(x - 1, y)});
                    if (int(curColor) == (int(C(x + 1, y + 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x - 1, y + 1), QPoint(x - 1, y)});
                }
                
                //---//
                
                if (int(curColor) == (int(C(x, y - 1)) + 15) ){
                    if(C(x, y + 1) != Color::Selection){
                        if (int(curColor) == (int(C(x, y + 2)) + 15))
                            m_availableMovesJ_V.append({QPoint(x, y + 2), QPoint(x, y + 1)});
                        if (int(curColor) == (int(C(x - 1, y + 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 1, y + 1), QPoint(x, y + 1)});
                        if (int(curColor) == (int(C(x + 1, y + 1)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 1, y + 1), QPoint(x, y + 1)});
                    }
                    if(C(x, y - 2) != Color::Selection){
                        if (int(curColor) == (int(C(x, y - 3)) + 15))
                            m_availableMovesJ_V.append({QPoint(x, y - 3), QPoint(x, y - 2)});
                        if (int(curColor) == (int(C(x - 1, y - 2)) + 15))
                            m_availableMovesJ_V.append({QPoint(x - 1, y - 2), QPoint(x, y - 2)});
                        if (int(curColor) == (int(C(x + 1, y - 2)) + 15))
                            m_availableMovesJ_V.append({QPoint(x + 1, y - 2), QPoint(x, y - 2)});
                    }
                }
                
                if (int(curColor) == (int(C(x, y - 2)) + 15) && C(x, y - 1) != Color::Selection){
                    if (int(curColor) == (int(C(x - 1, y - 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x - 1, y - 1), QPoint(x, y - 1)});
                    if (int(curColor) == (int(C(x + 1, y - 1)) + 15))
                        m_availableMovesJ_V.append({QPoint(x + 1, y - 1), QPoint(x, y - 1)});
                }
                
            }
            
		}
        
	}
    
    cout << "sizes1: " << endl;
    cout << m_availableMoves.size() << endl;
    cout << m_availableMovesJ_spaccatutto.size() << endl;
    cout << m_availableMovesJ_V.size() << endl;
    cout << m_availableMovesJ_H.size() << endl;
    
    m_availableMoves.append(m_availableMovesJ_spaccatutto);
    m_availableMoves.append(m_availableMovesJ_V);
    m_availableMoves.append(m_availableMovesJ_H);
    
    cout << "sizes2: " << endl;
    cout << m_availableMoves.size() << endl;
    
    
#undef C
	if (m_availableMoves.isEmpty()){
		m_board->clearSelection();
		m_gameState->setState(State::Finished); //TODO, forse va agginto un EndGameJob
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
                    m_availableMovesJ_spaccatutto.clear();
                    m_availableMovesJ_H.clear();
                    m_availableMovesJ_V.clear();
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
    
    
    auto d1=mossaSelezionata.first;
    auto d2=mossaSelezionata.second;
    //questo serve se facciamo una mossa che coinvolte il JollySpaccaTutto
    //forse la prima condizione è superflua e la possiamo togliere
    if (m_board->diamond(d2)->color() == Color::JollyScoppiaTutto && m_board->usoJollySpaccaTutto==1){
        
        //cerco dappertutto caramelle == d1
        for (y = 0; y < gridSize; ++y){
            for (x = 0; x < gridSize ; ++x){
                currentColor = C(x, y);
                if(currentColor == Color::Selection)
                    continue;
                if(currentColor != m_board->diamond(d1)->color())
                    continue;
                //se l'esecuzione arriva qui abbiamo trovato una caramella dello stesso
                //colore di quella che abbiamo scambiato col JollySpaccaTutto
                diamonds << QPoint(x, y);
            }
        }
        //ora metto flagJolly=0 in modo tale che nelle cascate non rientro più
        //in questa parte di ciclo
        m_board->usoJollySpaccaTutto=0;
        
        //rimuovo il jolly che è stato appena usato
        diamonds << d2;
        
    }
    
    //questo serve se facciamo una mossa che coinvolte il Jolly HorV
    
    for (y = 0; y < gridSize; ++y){
        for (x = 0; x < gridSize; ++x){
            //cerco il Jolly in tutto il quadro
            currentColor = C(x, y);
            if(int(currentColor) > 8 && int(currentColor) < 16){
                if (UseJollyHorV(x, y, 8)){
                    //se trovo il jolly H e appartiene a una tripletta -> cancello tutta
                    //la riga cui appartiene
                    for (x=0; x< gridSize; ++x)  diamonds << QPoint(x, y);
                }
            }
            if(int(currentColor) > 15 && int(currentColor) < 23){
                if (UseJollyHorV(x, y, 15)){
                    //se trovo il jolly V e appartiene a una tripletta -> cancello tutta
                    //la colonna cui appartiene
                    for (y=0; y< gridSize; ++y)  diamonds << QPoint(x, y);
                }
            }
        }
    }
    
    //ha senso cercare triplette solo se non ho fatto una mossa che coinvolge il jolly spacca tutto
    //perchè sarebbe ridondante (il jolly spacca tutto elimina già le evenutali triplette che formerei)
    
    if (!m_board->usoJollySpaccaTutto){
    
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
        
	}
    
    m_board->sizeDeleted=diamonds.size();
    
    //cout << "*********" << diamonds.size() << endl;
    
    //metto questa condizione perchè a diamonds vengono continuamente aggiunti
    //dei diamanti da eliminare fino a quando diamonds.size() non diventa zero.
    //in quest'ultimo caso però diamonds non contiene nessun punto, e quindi
    //se accedere al suo primo elemento causa segmentation fault.
    if (m_board->sizeDeleted>0)
    {
        m_board->colorDeleted=m_board->diamond(diamonds.at(0))->color();
        cout << "color deleted: " << int(m_board->colorDeleted) << " size: " << diamonds.size() << " in " << diamonds.at(0).x() << " " << diamonds.at(0).y() << endl;
    }

        
#undef C
	return diamonds;
}

const QList<pair<QPoint,QPoint>>& Game::availMoves() const{
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

bool Game::UseJollyHorV(const int x, const int y, int shift) const{
#define C(X, Y) (m_board->hasDiamond(QPoint(X, Y))? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
    Color currentColor;
    currentColor = C(x, y);
    
    bool c1=int(currentColor) == int(C(x + 1, y)) + shift && int(currentColor) == int(C(x + 2, y)) + shift;
    bool c2=int(currentColor) == int(C(x - 1, y)) + shift && int(currentColor) == int(C(x - 2, y)) + shift;
    bool c3=int(currentColor) == int(C(x - 1, y)) + shift && int(currentColor) == int(C(x + 1, y)) + shift;
    
    bool c4=int(currentColor) == int(C(x, y + 1)) + shift && int(currentColor) == int(C(x, y + 2)) + shift;
    bool c5=int(currentColor) == int(C(x, y - 1)) + shift && int(currentColor) == int(C(x, y - 2)) + shift;
    bool c6=int(currentColor) == int(C(x, y - 1)) + shift && int(currentColor) == int(C(x, y + 1)) + shift;
    
    return c1 || c2 || c3 || c4 || c5 || c6;
    
}
