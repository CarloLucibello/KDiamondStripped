#include "game.h"

#include <cassert>
using namespace std;

Game::Game(int seed, bool verbose)
	: m_gameState(new GameState)
	, m_gameParams(new GameParams)
	, m_board(new Board(seed))
    , m_verbose(verbose)
    , m_testBoard(new Board(seed))
    , m_testState(new GameState){Bag2attiva=-1;}

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

    m_jobQueue << Job::UpdateAvailableMoves;
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


//Checks amount of possible moves remaining. Questo e` fatto testando l´ effetto di ogni mossa su testBoard
void Game::getMoves(){
    
    uploadingmoves=1;
    
    m_availableMoves.clear();

    int oldPoints = m_gameState->points();
    int oldLiquirizie = m_board->count(CellMask::LIQUIRIZIA);
    int oldGelatine = m_board->count(CellMask::GELATINA);

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
                    *m_testState = *m_gameState;
                    *m_testBoard  = *m_board;
                    bool verbose = false;
                    swap(m_testState, m_gameState);
                    swap(m_testBoard, m_board);
                    swap(verbose, m_verbose);
                    m_board->swapDiamonds(point, dest); //ATTENZIONE questo swap non deve produrre animazioni


                    QVector<Figure> figuresToRemove;

                    auto figure1 = findFigure(point);
                    if(!figure1.isEmpty())
                        figuresToRemove.append(figure1);

                    auto figure2 = findFigure(dest);
                    if(!figure2.isEmpty())
                        figuresToRemove.append(figure2);

                    auto figCookie = findFigureCookie(point, dest);
                    if(!figCookie.isEmpty())
                        figuresToRemove.append(figCookie);

                    if(!figuresToRemove.isEmpty()){
                        removeFigures(figuresToRemove);

                        int newPoints = m_gameState->points();
                        int newLiquirizie = m_board->count(CellMask::LIQUIRIZIA);
                        int newGelatine = m_board->count(CellMask::GELATINA);

                        Move m(point, dest);
                        m.m_points = newPoints - oldPoints;
                        m.m_gelatine = oldGelatine - newGelatine;
                        m.m_liquirizie = oldLiquirizie - newLiquirizie;
                        m_availableMoves.append(m);
                    }

                    m_board->swapDiamonds(point, dest); //riswappo indietro. devo farlo prima di swappare le board
                    swap(m_testState, m_gameState);
                    swap(m_testBoard, m_board);
                    swap(verbose, m_verbose);
                }
            }
        }
    }
    
    //cout << "size" << endl;
    
    if (m_availableMoves.isEmpty()){
        cout << "NON CI SONO MOSSE DISPONILIBILI :(" << endl;
        cout << "CREO UN QUADRO DA CAPO, CONTINUA A GIOCARE :)" << endl;
        m_jobQueue.prepend(Job::NoMoves);
    }
    
    uploadingmoves=0;

    
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
        m_jobQueue << Job::SwapDiamonds;
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
        m_jobQueue << Job::SwapDiamonds;
	}
}

bool Game::executeFirstJob(){
	if(m_jobQueue.isEmpty()){
		return false;
	}
	//execute next job in queue
	const Job job = m_jobQueue.takeFirst();
	switch (job){
        case Job::SwapDiamonds: {
            if(m_verbose) cout <<"**** SWAPPING **** Job::SwapDiamonds" << endl;

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
            m_jobQueue << Job::RemoveFigures; //We already insert this here to avoid another conditional statement.
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
                    m_jobQueue.prepend(Job::RevokeSwapDiamonds);
				}
				else {
                    if(m_gameState->movesLeft() == 0){
                        m_jobQueue << Job::EndGame;
                    } else {
                        m_jobQueue << Job::UpdateAvailableMoves;
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

                //questa fa tutto il lavoro
                removeFigures(figuresToRemove);

                m_jobQueue.prepend(Job::FillGaps);
                if(m_verbose) printBoard();

                
                // lo ripulisco alla fine perché prima mi serve
                m_swappingDiamonds.clear();
			}
			break;
		}

        case Job::FillGaps:
            if(m_verbose) cout << "Job::FillGaps:" << endl;
                        
            // prima di far scendere le caramelle creo Bag2, nel punto più alto disponibile
            if (Bag2attiva>=0){
                for(int y = 0; y < m_board->gridSize(); ++y) {
                    int x=Bag2attiva;
                    if (!m_board->hasDiamond({x,y})){
                        m_board->rDiamond({x,y}) = m_board->spawnDiamond(Color::NoColor, JollyType::Bag2);
                        if(m_verbose){
                            cout    << "CREATO Bag2 in " << x << " " << y << " B2: " << Bag2attiva << endl;
                        }
                        break;
                    }
                }
            }
            
            
			//fill gaps
			m_board->fillGaps();
            if(m_verbose) printBoard();
            m_jobQueue.prepend(Job::RemoveFigures); //allow cascades (i.e. clear rows that have been formed by falling diamonds)
//			printBoard();
            
            //se ho scoppiato una busta, viene creata la Bag2, e bisogna andare a
            //scoppiare anche lei.
            //nel frattempo, lo scoppiaggio della prima busta ha fatto esplodere un quadrato 3 per 3 intorno
            //a lei, e tutte le caramelle, compresa la Bag2 appena creata, sono già scese visto che
            //ho appena usato fillGaps.
            if (Bag2attiva>=0){
                
                // dopo aver fatto scendere le caramelle non so più dove sta Bag2, quindi vado a cercarla
                for(int y = 0; y < m_board->gridSize(); ++y) {
                    int x=Bag2attiva;
                    //alcuni diamanti della linea potremmo averli già cancellati
                    if (m_board->hasDiamond({x,y})){
                        auto jtype = m_board->diamond({x,y})->jollyType();
                        if (jtype==JollyType::Bag2) removeJolly({x,y});
                    }
                }
                                
                m_jobQueue.prepend(Job::FillGaps);
                if(m_verbose) printBoard();
                
            }


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
    
    //L'if su m_board->hasDiamond(point) non dovrebbe essere superfluo?
    //io direi di si, ma se non ce lo metto a volte becco segmentation for
    //prova a toglierlo e usare i semi:
    //./kdiamond-stripped -p 10 -g 14 -q 1
    //mettendocelo dovrebbe stare tutto a posto ma dovremmo guardare meglio questa cosa.
    
    //L'if su uploadingmoves serve a questo:
    //qui dentro si entra anche nel momento in cui stiamo aggiornando le mosse disponibili
    //se c'è una mossa in cui si cancella una busta, senza questo if verrebbe attivato Bag2attiva
    //se ora questa mossa venisse scelta, questo non porterebbe a problemi in fillGaps
    //se però la mossa non venisse scelta, in resterebbe Bag2attiva >= 0
    //ma senza nessuna busta cancellata, e in fillGaps resteremmo intrappolati in un ciclo infinito
    
    if (m_board->hasDiamond(point) && uploadingmoves == 0){
        auto jtype = m_board->diamond(point)->jollyType();
        if (jtype == JollyType::Bag) Bag2attiva=point.x();
        if (jtype == JollyType::Bag2) Bag2attiva=-1;
    }
    
    
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
    if(jtype == JollyType::Bag || jtype == JollyType::Bag2){
        
        //non capisco come faccia a entrare qua dentro anche nel momento in cui sceglie la mossa da fare,
        //ma non rimuove i diamanti.
        //eppure già conosce il valore di Bag2attiva, che fisso solo quando rimuovo la Bag
        //questa cosa non mi da problemi, solo che non la capisco.

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


void Game::removeFigures(const QVector<Figure>& figuresToRemove){
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
                //il Bag ha un colore!
                //color = Color::NoColor;
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

void Game::getJollies(const Figure& fig, QVector<JollyType>& jtypes, QVector<QPoint> excludedPoints) const{
    for(auto& p : fig.points()){
        if(!excludedPoints.contains(p)){
            auto d = m_board->diamond(p);
            if(d->isJolly()){
                jtypes.append(d->jollyType());
            }
        }
    }
}



