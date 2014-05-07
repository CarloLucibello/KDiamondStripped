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
            QVector<QPoint> destinations;
            destinations.append(point + QPoint(1, 0));
            destinations.append(point + QPoint(0, 1));
            for(auto dest : destinations){
                if(m_board->hasDiamond(dest)){
                    m_board->swapDiamonds(point, dest); //ATTENZIONE questo swap non deve produrre animazioni
                    auto figure1 = findFigure(point).points();
                    auto figure2 = findFigure(dest).points();
                    //se la mossa ha successo
                    if(!(figure1.isEmpty() && figure2.isEmpty())){
                        Move mov(point, dest);
                        mov.m_toDelete = figure1 + figure2;
                        m_availableMoves.append(mov);
                    }
                    m_board->swapDiamonds(point, dest); //riswappo indietro
                }
            }
        }
    }

    if (m_availableMoves.isEmpty()){
		m_board->clearSelection();
		m_gameState->setState(State::Finished); //TODO, forse va agginto un EndGameJob
	}
}

//ritorna la riga verticale contenente il punto (escluso il punto stesso)
QVector<QPoint> Game::findRowV(const QPoint& point){
    QVector<QPoint> row;
    #define C(X, Y) (m_board->hasDiamond(QPoint(X, Y)) ? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
    Color currColor = m_board->diamond(point)->color();  //ATTENZIONE Non controllo che il colore sia valido (!=Selection)
    int yt = point.y() + 1;
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

//ritorna la riga orizzontale contenente il punto (escluso il punto stesso)
QVector<QPoint> Game::findRowH(const QPoint& point){
    QVector<QPoint> row;
    #define C(X, Y) (m_board->hasDiamond(QPoint(X, Y)) ? m_board->diamond(QPoint(X, Y))->color() : Color::Selection)
    Color currColor = m_board->diamond(point)->color();  //ATTENZIONE Non controllo che il colore sia valido (!=Selection)
    int xt = point.x() + 1;
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
			m_jobQueue << Job::RemoveFiguresJob; //We already insert this here to avoid another conditional statement.
            break;
		} //fall through

		case Job::RevokeSwapDiamondsJob:
//		cout<<"Job::RevokeSwapDiamondsJob" << endl;
			//invoke movement
			m_board->swapDiamonds(m_swappingDiamonds[0], m_swappingDiamonds[1]);
			break;

		case Job::RemoveFiguresJob: {
//				cout<<"Job::RemoveRowJob" << endl;
			const QVector<Figure> figuresToRemove = findFigures();
            cout<<"#Figures to remove " << figuresToRemove.size() << endl;
			if (figuresToRemove.isEmpty()){
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

//				/** Annoto i jolly che devo inserire
                QVector<QPoint> jPoint;
                QVector<JollyType> jType;
                QVector<Color> jColor;
                for(const auto& fig : figuresToRemove){
                    if(fig.size() > 3){
                        auto point = fig.points().last(); //TODO Per ora lo metto nell'ultimo punto
                        auto color = m_board->diamond(point)->color();
                        JollyType type;
                        if(fig.type() == FigureType::RowH && fig.size() == 4){
                            type = JollyType::H;
                        } else if(fig.type() == FigureType::RowV && fig.size() == 4){
                            type = JollyType::V;
                        } else if((fig.type() == FigureType::RowV || fig.type() == FigureType::RowH)
                            && fig.size() > 4){
                            type = JollyType::Cookie;
                        } else if(fig.type() == FigureType::LT){
                            type = JollyType::Bag;
                        }
                        jPoint += point;
                        jType += type;
                        jColor += color;
                    }
                }

                //Segno i punti ed elimino le figure
                for(const auto& fig : figuresToRemove){
                    m_gameState->addPoints(fig.size());//TODO cambiare addPoints per accettare una figura
                    //invoke remove animation, then fill gaps immediately after the animation
                    for(const QPoint& diamondPos: fig.points())
                        m_board->removeDiamond(diamondPos);
                }

                //Creo i Jolly
                for(int i = 0; i < jPoint.size(); ++i){
                    m_board->rDiamond(jPoint[i]) = m_board->spawnDiamond(jColor[i], jType[i]);
                }

				m_jobQueue.prepend(Job::FillGapsJob);
//				printBoard();
			}
			break;
		}

		case Job::FillGapsJob:
//		cout << "Job::FillGapsJob:" << endl;
			//fill gaps
			m_board->fillGaps();
			m_jobQueue.prepend(Job::RemoveFiguresJob); //allow cascades (i.e. clear rows that have been formed by falling diamonds)
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

QVector<Figure> Game::findFigures(){
	QVector<Figure> diamonds;
	const int gridSize = m_board->gridSize();
	QVector<bool> inFigure(gridSize * gridSize, false);
	for (QPoint point; point.x() < gridSize; ++point.rx()){
		for (point.ry() = 0; point.y() < gridSize ; ++point.ry()){
            //controllo che ci sia un diamante e che non sia già parte di una figura
            if(m_board->hasDiamond(point) && !inFigure[point.x() + gridSize * point.y()]){
                auto figure = findFigure(point);
                for(auto& p : figure.points()){
                    inFigure[p.x() + gridSize * p.y()] = true;
                }

                if (figure.size() > 0){
                    cout << "------------tipo di figura: " << int(figure.type()) << " in " << point.x() << " " << point.y() << endl;
                    diamonds += figure;
                }
            }
        }
	}
	return diamonds;
}

Figure Game::findFigure(QPoint point){
    auto rH = findRowH(point);
    auto rV = findRowV(point);

    FigureType type;
    QVector<QPoint> points;
    //Controllo che figura ho trovato. Si può rendere pìù efficiente
    if(rH.size() >= 2 && rV.size() < 2){
    //riga orizzontale
        points.append(point);
        points += rH;
        type = FigureType::RowH;
        //controllo se si crea una T o una L
        for(auto p : rH){
            auto rV2 = findRowV(p);
            if(rV2.size() >= 2){
                points += rV2;
                type = FigureType::LT;
                //ATTENZIONE si possono avere più file verticali o posso
                //mettere un break?
                //break;
            }
        }
    } else if(rH.size() < 2 && rV.size() >= 2){
    // riga verticale
        points.append(point);
        points += rV;
        type = FigureType::RowV;
        //controllo se si crea una T o una L
        for(auto p : rV){
            auto rH2 = findRowH(p);
            if(rH2.size() >= 2){
                points += rH2;
                type = FigureType::LT;
                //ATTENZIONE si possono avere più file verticali o posso
                //mettere un break?
                //break;
            }
        }

    } else if(rH.size() >= 2 && rV.size() >= 2){
        //ho trovato una T o una L
        points.append(point);
        points += rH;
        points += rV;
        type = FigureType::LT;
        //ATTENZIONE non controllo la creazione di figure più complesse
        // che si potrebbero formare quando calano i diamanti dall'alto

        //qua se rH.size()=2 e rV.size()=2 allora devi creare una busta in point
        //se però, per esempio, rH.size()=3 e rV.size()=2, c'è un conflitto:
        //-creo un jolly verticale in point?
        //-creo sempre una busta in point?
    }

    return Figure(points, type);
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
