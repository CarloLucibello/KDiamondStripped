#include "game.h"

#include <cassert>
using namespace std;

Game::Game(int seed, bool verbose)
	: m_gameState(new GameState)
	, m_gameParams(new GameParams)
	, m_board(new Board(seed))
    , m_verbose(verbose)
    , m_testBoard(new Board(seed))
    , m_testState(new GameState){}

Game::~Game(){
    delete m_board;
	delete m_gameState;
	delete m_gameParams;

    delete m_testBoard;
    delete m_testState;
}

void Game::startNewGame(){
    if(m_verbose){
        cout << "Starting new game ...." << endl;
    }
    m_gameState->startNewGame(m_gameParams);
    m_board->startNewGame(m_gameParams);

    m_testState->startNewGame(m_gameParams);
    m_testBoard->startNewGame(m_gameParams);
    if(m_verbose){
        printBoard();
        cout << endl;
    }

    m_jobQueue.push_back(Job::UpdateAvailableMoves);
    executeJobs();
    if(m_verbose)
        cout << "....done!"  << endl;

}

void Game::setParamsPath(string paramsPath){
        m_gameParams->setParamsPath(paramsPath);
}


void Game::setLevel(const int level){
    m_gameParams->setLevel(level);
}


// Checks amount of possible moves remaining.
// Questo e` fatto testando l´ effetto di ogni mossa su testBoard.
// Non ha effetti sulla board vera, ovvero quella visualizzata.
// Praticamente simula la mossa.
void Game::getMoves(){
    
    m_availableMoves.clear();

    int oldPoints = m_gameState->points();
    int oldLiquirizie = m_board->count(CellMask::LIQUIRIZIA);
    int oldGelatine = m_board->count(CellMask::GELATINA);

    const int gridSize = m_board->gridSize();
    for (Point point; point.x() < gridSize - 1; ++point.rx()){
        for (point.ry() = 0; point.y() < gridSize - 1; ++point.ry()){
            if(!m_board->hasDiamond(point)) continue;
             //guardo solo a sinistra e in basso, gli altri casi sono considerati
             //dagli altri punti
            vector<Point> destinations;
            destinations.push_back(point + Point(1, 0));
            destinations.push_back(point + Point(0, 1));
            for(auto dest : destinations){
                if(m_board->hasDiamond(dest)){
                    *m_testState = *m_gameState;
                    *m_testBoard  = *m_board;
                    bool verbose = false;
                    swap(m_testState, m_gameState);
                    swap(m_testBoard, m_board);
                    swap(verbose, m_verbose);
                    m_board->swapDiamonds(point, dest); //ATTENZIONE questo swap non deve produrre animazioni


                    vector<Figure> figuresToRemove;

                    auto figure1 = findFigure(point);
                    if(!figure1.empty())
                        figuresToRemove.push_back(figure1);

                    auto figure2 = findFigure(dest);
                    if(!figure2.empty())
                        figuresToRemove.push_back(figure2);

                    auto figCookie = findFigureCookie(point, dest);
                    if(!figCookie.empty())
                        figuresToRemove.push_back(figCookie);

                    if(!figuresToRemove.empty()){
                        //vedo tutto quello che succede senza generare nuovi diamanti
                        while(!figuresToRemove.empty()){
                            removeFigures(figuresToRemove);
                            m_board->dropDiamonds(); //uso questa al posto di fillGaps() per non generare nuovi diamanti
                            figuresToRemove = findFigures();
                        }

                        int newPoints = m_gameState->points();
                        int newLiquirizie = m_board->count(CellMask::LIQUIRIZIA);
                        int newGelatine = m_board->count(CellMask::GELATINA);

                        Move m(point, dest);
                        m.m_points = newPoints - oldPoints;
                        m.m_gelatine = oldGelatine - newGelatine;
                        m.m_liquirizie = oldLiquirizie - newLiquirizie;
                        m_availableMoves.push_back(m);
                    }

                    //riswappo indietro. devo farlo prima di swappare le board
                    m_board->swapDiamonds(point, dest);
                    swap(m_testState, m_gameState);
                    swap(m_testBoard, m_board);
                    swap(verbose, m_verbose);
                }
            }
        }
    }
    
    if (m_availableMoves.empty()){
        if (m_verbose){
            cout << "NON CI SONO MOSSE DISPONIBILI :(" << endl;
            cout << "CREO UN QUADRO DA CAPO, CONTINUA A GIOCARE :)" << endl;
        }
        m_jobQueue.push_front(Job::NoMoves);
    }   
}

//ritorna la riga verticale contenente il punto (escluso il punto stesso)
vector<Point> Game::findRowV(const Point& point){
    vector<Point> row;
    #define C(X, Y) (m_board->hasDiamond(Point(X, Y)) ? m_board->diamond(Point(X, Y))->color() : Color::Selection)
    Color currColor = m_board->diamond(point)->color();
    // Non faccio terzine etc.. di buste e cookie
    if(currColor == Color::Selection
        || currColor == Color::NoColor){
        return row;
    }int yt = point.y() + 1;

    const int x = point.x();
    while(C(x, yt) == currColor){ //ciclo verso il basso
        row.push_back(Point(x, yt));
        yt++;
    }

    yt = point.y() - 1;
    while(C(x, yt) == currColor){ //ciclo verso l'alto
        row.push_back(Point(x, yt));
        yt--;
    }
    #undef C
    return row;
}

//ritorna la riga orizzontale contenente il punto (escluso il punto stesso)
vector<Point> Game::findRowH(const Point& point){
    vector<Point> row;
    #define C(X, Y) (m_board->hasDiamond(Point(X, Y)) ? m_board->diamond(Point(X, Y))->color() : Color::Selection)
    Color currColor = m_board->diamond(point)->color();
    //Non faccio terzine etc.. di buste e cookie
    if(currColor == Color::Selection
        || currColor == Color::NoColor){
        return row;
    }
    int xt = point.x() + 1;
    const int y = point.y();
    while(C(xt, y) == currColor){ //ciclo verso destra
        row.push_back(Point(xt, y));
        xt++;
    }

    xt = point.x() - 1;
    while(C(xt, y) == currColor){ //ciclo verso sinistra
        row.push_back(Point(xt, y));
        xt--;
    }
    #undef C
    return row;
}


void Game::clickDiamond(const Point& point){
	if (m_gameState->state() != State::Playing)
		return;
	//do not allow more than two selections
	const bool isSelected = m_board->hasSelection(point);
    if (!isSelected && m_board->selections().size() == 2)
		return;
	//select only adjacent diamonds (i.e. if a distant diamond is selected, deselect the first one)
    for(const Point& point2 : m_board->selections()){
        const int diff = abs(point2.x() - point.x()) + abs(point2.y() - point.y());
		if (diff > 1)
			m_board->setSelection(point2, false);
	}
	//toggle selection state
	m_board->setSelection(point, !isSelected);
    if (m_board->selections().size() == 2){
        m_jobQueue.push_back(Job::SwapDiamonds);
		executeJobs();
    }
}

void Game::dragDiamond(const Point& point, const Point& direction){
	//direction must not be null, and must point along one axis
	if ((direction.x() == 0) ^ (direction.y() == 0)){
		//find target indices
        const Point point2 = point + direction;
		if(!m_board->hasDiamond(point2))
			return;
		//simulate the clicks involved in this operation
		m_board->clearSelection();
		m_board->setSelection(point, true);
		m_board->setSelection(point2, true);
        m_jobQueue.push_back(Job::SwapDiamonds);
	}
}

bool Game::executeFirstJob(){
    if(m_jobQueue.empty()){
		return false;
	}
	//execute next job in queue
    const Job job = m_jobQueue.front();
    m_jobQueue.pop_front();
	switch (job){
        case Job::SwapDiamonds: {
            if(m_verbose) cout <<"**** SWAPPING **** Job::SwapDiamonds" << endl;

            assert(m_board->selections().size() == 2);
            const vector<Point> points = m_board->selections();
            //m_swappingDiamonds contiene i diamanti che stiamo scambiando
			m_swappingDiamonds = points;
            if(m_verbose){
                vector<Point> m_swappingDiamonds_vec;
                m_swappingDiamonds_vec.push_back(m_swappingDiamonds[0]);
                m_swappingDiamonds_vec.push_back(m_swappingDiamonds[1]);
                printSelection(m_swappingDiamonds_vec);
            }
            m_board->swapDiamonds(points[0], points[1]);
            m_jobQueue.push_back(Job::RemoveFigures); //We already insert this here to avoid another conditional statement.
            break;
		} //fall through

        case Job::RevokeSwapDiamonds:
            if(m_verbose) {
                cout<<"***** Job::RevokeSwapDiamonds" << endl;
            }
			//invoke movement

			m_board->swapDiamonds(m_swappingDiamonds[0], m_swappingDiamonds[1]);
			break;

        case Job::RemoveFigures: {
            if(m_verbose) cout<<"Job::RemoveFigures" << endl;
            vector<Figure> figuresToRemove = findFigures();

            //Controllo se ho swappato un Cookie
            if(!m_swappingDiamonds.empty()){
                auto fig = findFigureCookie(m_swappingDiamonds[0], m_swappingDiamonds[1]);
                if(!fig.empty()){
                    if(m_verbose) cout << "**** Swappo Cookie" << endl;
                    figuresToRemove.push_back(fig);
                }
			}


            if (figuresToRemove.empty()){
				//no diamond rows were formed by the last move -> revoke movement (unless we are in a cascade)
                if (!m_swappingDiamonds.empty()){
                    m_jobQueue.push_front(Job::RevokeSwapDiamonds);
				}
				else {
                    if(m_gameState->movesLeft() == 0){
                        m_jobQueue.push_back(Job::EndGame);
                    } else {
                        m_jobQueue.push_back(Job::UpdateAvailableMoves);
                    }
                }
			}
			else{ //C'è qualcosa da rimuovere


				//all moves may now be out-dated - flush the moves list
                if (!m_availableMoves.empty()){
					m_availableMoves.clear();
				}
				// Controllo se sto swappando e dato che lo swap ha avuto successo
                // incremento il numero di mosse ed elimino la selezione
                if(!m_swappingDiamonds.empty()){
                    m_gameState->updateMovesLeft();
                    m_board->clearSelection();
                }

                //questa fa tutto il lavoro
                removeFigures(figuresToRemove);

                if(isWon()){
                    m_jobQueue.push_front(Job::EndGame);
                } else {
                    m_jobQueue.push_front(Job::FillGaps);
                }

                if(m_verbose) printBoard();

                
                // lo ripulisco alla fine perché prima mi serve
                m_swappingDiamonds.clear();
			}
			break;
		}

        case Job::FillGaps:
            if(m_verbose)
                cout << "Job::FillGaps:" << endl;
			//fill gaps
			m_board->fillGaps();
            if(m_verbose)
                printBoard();
            m_jobQueue.push_front(Job::RemoveFigures); //allow cascades (i.e. clear rows that have been formed by falling diamonds)
            break;


        case Job::UpdateAvailableMoves:
            if(m_verbose) {
                cout <<"Job::UpdateAvailableMoves:" << endl;
                printState();
            }
			if (m_gameState->state() != State::Finished)
				getMoves();
			break;

        case Job::EndGame:
            if(m_verbose)cout <<"Job::EndGame" << endl;
            m_gameState->setState(State::Finished);
            if(m_verbose) printState();
			break;

        case Job::NoMoves:
            if(m_verbose)
                cout <<"***  Job::NoMoves *** Respawning Diamonds" << endl;
            m_board->spawnDiamonds();
            getMoves();
            break;
	}

	return true;
}

vector<Point> Game::findDiamonds(Color color){
    vector<Point> points;
    for(int y = 0; y < m_board->gridSize(); ++y) {
        for(int x = 0; x < m_board->gridSize(); ++x) {
            //alcuni diamanti potremmo averli già cancellati
            if (m_board->hasDiamond({x,y})){
                Color colorePoint = m_board->diamond({x,y})->color();
                if(colorePoint == color){
                    points.push_back(Point(x, y));
                }
            }
        }
    }
    return points;
}

void Game::removeDiamond(const Point& point){
    if(m_verbose) cout << "rimuovo diamante"  <<" in " << point.x() << " " << point.y() << endl;

    m_gameState->addPoints(1);
    m_board->removeDiamond(point);
}


void Game::removeJolly(const Point& point){
    if(m_verbose) cout << "SCOPPIO Jolly"  <<" in " << point.x() << " " << point.y() << endl;
    auto jtype = m_board->diamond(point)->jollyType();
    removeDiamond(point);

    //Inserisco lo scoppiaggio di un jolly H
    if(jtype == JollyType::H){
        int y = point.y();
        for(int x = 0; x < m_board->gridSize(); ++x) {

            //alcuni diamanti della linea potremmo averli già cancellati
            if (m_board->hasDiamond({x,y})){
                if (m_board->diamond({x,y})->isJolly())
                    removeJolly({x,y});
                else
                    removeDiamond({x,y});
            }

        }
    }
    
    //Inserisco lo scoppiaggio di un jolly V
    if(jtype == JollyType::V){
        int x = point.x();
        for(int y = 0; y < m_board->gridSize(); ++y) {

            //alcuni diamanti della linea potremmo averli già cancellati
            if (m_board->hasDiamond({x,y})){
                if (m_board->diamond({x,y})->isJolly())
                    removeJolly({x,y});
                else
                    removeDiamond({x,y});
            }

        }
    }

    //Inserisco lo scoppiaggio di una busta
    if(jtype == JollyType::Bag || jtype == JollyType::Bag2){
        //RIMUOVE I DIAMANTI in un quadrato intorno alla busta
        int px = point.x();
        int py = point.y();
        for(int y = py - 1; y <= py + 1; ++y) {
            for(int x = px - 1; x <= px + 1; ++x) {
                //alcuni diamanti del quadrato 3*3 potremmo averli già cancellati
                if (m_board->hasDiamond({x,y})){
                    if (m_board->diamond({x,y})->isJolly())
                        removeJolly({x,y});
                    else
                        removeDiamond({x,y});
                }
            }
        }

        // Creo il secondo stadio della bag.
        // Sara´ scoppiata automaticamente al prossimo removeFigures
        if(jtype == JollyType::Bag){
            m_board->rDiamond(point) = m_board->spawnDiamond(Color::NoColor, JollyType::Bag2);
        }
    }

    // Questa parte viene chiamata solo quando il jolly viene
    // scoppiata dallo scoppiaggio di una riga intera o di un quadrato,
    // quindi viene sprecato.
    // (lo scoppiaggio del cookie viene fatto a parte in findFigureCookie)
    if(jtype == JollyType::Cookie){
        if(m_verbose) cout <<"****** sprecato cookie "<< endl;
        removeDiamond(point);
    }
}


void Game::removeFigures(const vector<Figure>& figuresToRemove){
    // Annoto i jolly che devo inserire
    vector<Point> jPoint;
    vector<JollyType> jType;
    vector<Color> jColor;
    for(const auto& fig : figuresToRemove){
        if(fig.size() > 3 && fig.type() != FigureType::None){
            // la figura None non produce Jolly (è il caso del Cookie)
            
            // creo il jolly nel punto in cui sposto il diamante
            // se creo un jolly durante una valanga lo
            // creo nell'ultimo punto di points
            Point point = fig.points().back(); //assegnazione di default
            if (!m_swappingDiamonds.empty()){
                //Se ho appena fatto la mossa la figura contenerra'
                // sicuramente uno e un solo punto swappato
                //assert(fig.points().contains(puntiSwappati[0])|| fig.points().contains(puntiSwappati[1]));
                point = contains(fig.points(), m_swappingDiamonds[0]) ? m_swappingDiamonds[0] : m_swappingDiamonds[1];
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
            }
            //ATTENZIONE che si ricada sempre in una delle condizioni precedenti TODO mettere un assert
            jPoint.push_back(point);
            jType.push_back(type);
            jColor.push_back(color);
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
        for(const Point& diamondPos: fig.points()){
            if(m_board->hasDiamond(diamondPos)){
                //potrebbe essere (casi rari) che era già stato scoppiato
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
}


void Game::executeJobs(){
    bool nonEmptyList = true;
    while(nonEmptyList){
        nonEmptyList = executeFirstJob();
    }
}

vector<Figure> Game::findFigures(){
    vector<Figure> diamonds;
	const int gridSize = m_board->gridSize();
    vector<bool> inFigure(gridSize * gridSize, false);
    for (Point point; point.x() < gridSize; ++point.rx()){
		for (point.ry() = 0; point.y() < gridSize ; ++point.ry()){
            //controllo che ci sia un diamante e che non sia già parte di una figura
            if(m_board->hasDiamond(point) && !inFigure[point.x() + gridSize * point.y()]){
                auto figure = findFigure(point);
                for(auto& p : figure.points()){
                    inFigure[p.x() + gridSize * p.y()] = true;
                }
                if (figure.size() > 0){
                    diamonds.push_back(figure);
                }
            }
        }
    }
	return diamonds;
}

Figure Game::findFigure(Point point){
    FigureType type;
    vector<Point> points;

    //Considero il secondo stadio della busta come una figura perche' va scoppiato
    if(m_board->diamond(point)->jollyType() == JollyType::Bag2){
        type = FigureType::Bag2;
        points.push_back(point);
        return Figure(points, type);

    }

    auto rH = findRowH(point);
    auto rV = findRowV(point);

    //Controllo che figura ho trovato. Si può rendere pìù efficiente
    if(rH.size() >= 2 && rV.size() < 2){
    //riga orizzontale
        points.push_back(point);
        appendTo(points, rH);
        type = FigureType::RowH;
        //controllo se si crea una T o una L
        for(auto p : rH){
            auto rV2 = findRowV(p);
            if(rV2.size() >= 2){
                appendTo(points, rV2);
                type = FigureType::LT;
                //ATTENZIONE si possono avere più file verticali o posso
                //mettere un break?
                //direi che non si possono avere più righe verticali: al massimo
                //se ne forma una
                break;
            }
        }
    } else if(rH.size() < 2 && rV.size() >= 2){
    // riga verticale
        points.push_back(point);
        appendTo(points, rV);
        type = FigureType::RowV;
        //controllo se si crea una T o una L
        for(auto p : rV){
            auto rH2 = findRowH(p);
            if(rH2.size() >= 2){
                appendTo(points, rH2);
                type = FigureType::LT;
                //ATTENZIONE si possono avere più file verticali o posso
                //mettere un break?
                break;
            }
        }

    } else if(rH.size() >= 2 && rV.size() >= 2){
        //ho trovato una T o una L
        points.push_back(point);
        appendTo(points,  rH);
        appendTo(points, rV);
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

const vector<Move>& Game::availMoves() const{
    return m_availableMoves;
}

bool Game::isFinished() const{
    return State::Finished == m_gameState->state();
}

bool Game::isWon() const {
    bool targetPoints = m_gameState->points() >= m_gameParams->points();
    bool targetGelatina = m_board->count(CellMask::GELATINA) == 0;
    bool targetLiquirizia = m_gameParams->targetLiquirizia() ? m_board->count(CellMask::LIQUIRIZIA) == 0 : true;
    return targetPoints && targetLiquirizia && targetGelatina;
}

int Game::points() const {
    return m_gameState->points();
}

//Controlla se in dei due punti c'è un cookie e in caso
//restituisce una figura con il cookie + tutti i diamanti del
//colore dell'altro punto. In caso contrario restituisc una figura vuota;
Figure Game::findFigureCookie(Point p1, Point p2){
    Figure fig;
    auto d1 = m_board->diamond(p1);
    auto d2 = m_board->diamond(p2);

    if(d1->jollyType() == JollyType::Cookie && !(d2->jollyType() == JollyType::Cookie)){
        auto pointsToRem = findDiamonds(d2->color());
        pointsToRem.push_back(p1); //aggiungo il cookie
        fig = Figure(pointsToRem, FigureType::None);
        //La figura deve essere di typo None altrimente poi si crea un jolly

    }
    else if(d2->jollyType() == JollyType::Cookie && !(d1->jollyType() == JollyType::Cookie)){
        auto pointsToRem = findDiamonds(d1->color());
        pointsToRem.push_back(p2); //aggiungo il cookie
        fig = Figure(pointsToRem, FigureType::None);
        //La figura deve essere di typo None altrimente poi si crea un jolly
    }

    return fig;
}

void Game::getJollies(const Figure& fig, vector<JollyType>& jtypes, vector<Point> excludedPoints) const{
    for(auto& p : fig.points()){
        if(!contains(excludedPoints, p)){
            auto d = m_board->diamond(p);
            if(d->isJolly()){
                jtypes.push_back(d->jollyType());
            }
        }
    }
}
