#include "game.h"

#include <cassert>
using namespace std;

Game::Game(int seed, bool verbose)
	: m_gameState(new GameState)
	, m_gameParams(new GameParams)
	, m_board(new Board(seed))
    , m_verbose(verbose){
}

Game::~Game(){
    delete m_board;
	delete m_gameState;
	delete m_gameParams;
}

void Game::startNewGame(){
    if(m_verbose){
        cout << "Starting new game ...." << endl;
    }
    m_gameState->startNewGame(m_gameParams);
    m_board->setParams(m_gameParams);
    m_board->startNewGame();
    if(m_verbose){
        printBoard();
        cout << endl;
    }

    m_jobQueue << Job::UpdateAvailableMovesJob;
    executeJobs();
    if(m_verbose)
        cout << "....done!"  << endl;

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

                    auto figCookie = findFigureCookie(point, dest);
                    if(!figCookie.isEmpty()){
                        Move mov(point, dest);
                        mov.m_toDelete = figCookie.points();
                        m_availableMoves.append(mov);
                        m_board->swapDiamonds(point, dest); //riswappo indietro
                        continue; // non c'è bisogno di guardare se faccio figure
                    }

                    auto figure1 = findFigure(point);
                    auto figure2 = findFigure(dest);

                    //se la mossa crea figure
                    if(!(figure1.isEmpty() && figure2.isEmpty())){
                        Move mov(point, dest);
                        mov.m_toDelete += figure1.points() + figure2.points();
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
    //Non faccio terzine etc.. di buste e cookie
    if(currColor == Color::Selection
        || currColor == Color::NoColor){
        return row;
    }int yt = point.y() + 1;

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

    //Non faccio terzine etc.. di buste e cookie
    if(currColor == Color::Selection
        || currColor == Color::NoColor){
        return row;
    }
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
            if(m_verbose) cout <<"**** SWAPPING **** Job::SwapDiamondsJob" << endl;

			assert(m_board->selections().count() == 2);
			const QList<QPoint> points = m_board->selections();
			m_swappingDiamonds = points;
            if(m_verbose){
                //printSelection({m_swappingDiamonds[0], m_swappingDiamonds[1]});
                QVector<QPoint> m_swappingDiamonds_vec;
                m_swappingDiamonds_vec.append(m_swappingDiamonds[0]);
                m_swappingDiamonds_vec.append(m_swappingDiamonds[1]);
                printSelection(m_swappingDiamonds_vec);
            }
            m_board->swapDiamonds(points[0], points[1]);
			m_jobQueue << Job::RemoveFiguresJob; //We already insert this here to avoid another conditional statement.
            break;
		} //fall through

		case Job::RevokeSwapDiamondsJob:
            if(m_verbose) {
                cout<<"***** Job::RevokeSwapDiamondsJob " << endl;
            }
			//invoke movement

			m_board->swapDiamonds(m_swappingDiamonds[0], m_swappingDiamonds[1]);
			break;

		case Job::RemoveFiguresJob: {
            if(m_verbose) cout<<"Job::RemoveFiguresJob" << endl;
			QVector<Figure> figuresToRemove = findFigures();

            //Controllo se ho swappato un Cookie
			if(!m_swappingDiamonds.isEmpty()){
                auto fig = findFigureCookie(m_swappingDiamonds[0], m_swappingDiamonds[1]);
                if(!fig.isEmpty()){
                    if(m_verbose) cout << "**** Swappo Cookie" << endl;
                    figuresToRemove += fig;
                }
			}


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
                    m_board->clearSelection();
				}

//				/** Annoto i jolly che devo inserire
                QVector<QPoint> jPoint;
                QVector<JollyType> jType;
                QVector<Color> jColor;
                for(const auto& fig : figuresToRemove){
                    if(fig.size() > 3 && fig.type() != FigureType::None){//la figura None non produce Jolly (è il caso del Cookie)
                        //cout << "-------------------------------------SIZE DELLA FIGURA CHE SCOPPIO: " << fig.size() << endl;
                        
                        // creo il jolly nel punto in cui sposto il diamante
                        // se creo un jolly durante una valanga lo
                        // creo nell'ultimo punto di points
                        QPoint point = fig.points().last(); //assegnazione di default
                        if (!m_swappingDiamonds.isEmpty()){
                            //Se ho appena fatto la mossa la figura contenerra'
                            // sicuramente uno e un solo punto swappato
                            //assert(fig.points().contains(puntiSwappati[0])|| fig.points().contains(puntiSwappati[1]));
                            point = fig.points().contains(m_swappingDiamonds[0]) ? m_swappingDiamonds[0] : m_swappingDiamonds[1];

                        }

                        auto color = m_board->diamond(point)->color();
                        JollyType type;
                        if(fig.type() == FigureType::RowH && fig.size() == 4){
                            type = JollyType::H;
                        } else if(fig.type() == FigureType::RowV && fig.size() == 4){
                            type = JollyType::V;
                        } else if((fig.type() == FigureType::RowV || fig.type() == FigureType::RowH)
                            && fig.size() > 4){
                            type = JollyType::Cookie;
                            color = Color::NoColor;
                        } else if(fig.type() == FigureType::LT){
                            type = JollyType::Bag;
                            color = Color::NoColor;
                        }
                        //ATTENZIONE che si ricada sempre in una delle condizioni precedenti TODO mettere un assert
                        jPoint += point;
                        jType += type;
                        jColor += color;
                    }
                }

                //Segno i punti ed elimino le figure
                if(m_verbose) {
                    printBoard();
                    cout<<"### Removing " << figuresToRemove.size() <<" Figures" << endl;
                }
                for(const auto& fig : figuresToRemove){
                    if(m_verbose) {
                        cout << "### Removing " << endl;
                        m_board->printSelection(fig.points());
                    }
                    //invoke remove animation, then fill gaps immediately after the animation
                    for(const QPoint& diamondPos: fig.points()){
                        if(m_board->hasDiamond(diamondPos)){ //potrebbe essere (casi rari) che era già stato scoppiato
                            if(m_board->diamond(diamondPos)->isJolly()){
                                removeJolly(diamondPos);
                            }
                            else {
                                removeDiamond(diamondPos);
                            }
                        }
                    }
                }

                for(int i = 0; i < jPoint.size(); ++i){
                    m_board->rDiamond(jPoint[i]) = m_board->spawnDiamond(jColor[i], jType[i]);
                    if(m_verbose){
                        cout    << "CREATO Jolly: " << int(jType[i]) << " color: " << int(jColor[i])
                                << " in " << jPoint[i].x() << " " << jPoint[i].y() << endl;
                    }
                }
				m_jobQueue.prepend(Job::FillGapsJob);
                if(m_verbose) printBoard();

                // lo ripulisco alla fine perché prima mi serve
                m_swappingDiamonds.clear();
			}
			break;
		}

		case Job::FillGapsJob:
             if(m_verbose) cout << "Job::FillGapsJob:" << endl;
			//fill gaps
			m_board->fillGaps();
            if(m_verbose) printBoard();
			m_jobQueue.prepend(Job::RemoveFiguresJob); //allow cascades (i.e. clear rows that have been formed by falling diamonds)
//			printBoard();
			break;

		case Job::UpdateAvailableMovesJob:
            if(m_verbose) {
                cout <<"Job::UpdateAvailableMovesJob:" << endl;
                printState();
            }
			if (m_gameState->state() != State::Finished)
				getMoves();
			break;

		case Job::EndGameJob:
            if(m_verbose)cout <<"Job::EndGameJob" << endl;
            m_gameState->setState(State::Finished);
            if(m_verbose) printState();
			break;
	}

	return true;
}

QVector<QPoint> Game::findDiamonds(Color color){
    QVector<QPoint> points;
    for(int y = 0; y < m_board->gridSize(); ++y) {
        for(int x = 0; x < m_board->gridSize(); ++x) {
            //alcuni diamanti potremmo averli già cancellati
            if (m_board->hasDiamond({x,y})){
                Color colorePoint = m_board->diamond({x,y})->color();
                if(colorePoint == color){
                    points += QPoint(x, y);
                }
            }
        }
    }
    return points;
}

void Game::removeDiamond(const QPoint& point){
    if(m_verbose) cout << "rimuovo diamante"  <<" in " << point.x() << " " << point.y() << endl;
    m_gameState->addPoints(1);
    m_board->removeDiamond(point);
}


void Game::removeJolly(const QPoint& point){
    if(m_verbose) cout << "SCOPPIO Jolly"  <<" in " << point.x() << " " << point.y() << endl;
    auto jtype = m_board->diamond(point)->jollyType();
    removeDiamond(point);


    //TODO Che succede se incontro un Jolly? Lo esplodo come jolly?

    //direi di si
    if(jtype == JollyType::H){
        int y = point.y();
        for(int x = 0; x < m_board->gridSize(); ++x) {

            //alcuni diamanti della linea potremmo averli già cancellati
            if (m_board->hasDiamond({x,y})){
                if (m_board->diamond({x,y})->isJolly()) removeJolly({x,y});
                else removeDiamond({x,y});
            }

        }
    }

    if(jtype == JollyType::V){
        int x = point.x();
        for(int y = 0; y < m_board->gridSize(); ++y) {

            //alcuni diamanti della linea potremmo averli già cancellati
            if (m_board->hasDiamond({x,y})){
                if (m_board->diamond({x,y})->isJolly()) removeJolly({x,y});
                else removeDiamond({x,y});
            }

        }
    }

    //Inserisco lo scoppiaggio di una busta
    if(jtype == JollyType::Bag){
        int px = point.x();
        int py = point.y();
        for(int y = py - 1; y <= py + 1; ++y) {
            for(int x = px - 1; x <= px + 1; ++x) {

                //alcuni diamanti del quadrato 3*3 potremmo averli già cancellati
                if (m_board->hasDiamond({x,y})){
                    if (m_board->diamond({x,y})->isJolly()) removeJolly({x,y});
                    else removeDiamond({x,y});
                }

            }
        }
    }

    //Questa parte viene chiamata solo quando il jolly viene
    //scoppiata dallo scoppiaggio di una riga intera o di un quadrato,
    //quindi viene sprecato
    if(jtype == JollyType::Cookie){
        if(m_verbose) cout <<"****** sprecato cookie "<< endl;
        removeDiamond(point);
    }
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
                //direi che non si possono avere più righe verticali: al simo
                //se ne forma una
                break;
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
                break;
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

//Controlla se in dei due punti c'è un cookie e in caso
//restituisce una figura con il cookie + tutti i diamanti del
//colore dell'altro punto. In caso contrario restituisc una figura vuota;
Figure Game::findFigureCookie(QPoint p1, QPoint p2){
    Figure fig;
    auto d1 = m_board->diamond(p1);
    auto d2 = m_board->diamond(p2);

    if(d1->jollyType() == JollyType::Cookie && !(d2->jollyType() == JollyType::Cookie)){
        auto pointsToRem = findDiamonds(d2->color());
        pointsToRem.append(p1); //aggiungo il cookie
        fig = Figure(pointsToRem, FigureType::None); //La figura deve essere di typo None altrimente poi si crea un jolly

    }
    else if(d2->jollyType() == JollyType::Cookie && !(d1->jollyType() == JollyType::Cookie)){
        auto pointsToRem = findDiamonds(d1->color());
        pointsToRem.append(p2); //aggiungo il cookie
        fig = Figure(pointsToRem, FigureType::None); //La figura deve essere di typo None altrimente poi si crea un jolly
    }

    return fig;
}



