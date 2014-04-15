#include "gamestate.h"
#include <QTime>

#include <iostream> //DEBUG
using namespace std;
GameStatePrivate::GameStatePrivate()
	//these should be the same values as in GameState::startNewGame
	: m_mode(Mode::UntimedGame)
	, m_points(0)
	, m_cascade(0)
    , m_state(State::Finished) {}

GameStatePrivate::~GameStatePrivate(){}

GameState::GameState()
	: p(new GameStatePrivate) {}


GameState::~GameState(){
	delete p;
}

Mode GameState::mode() const{
	return p->m_mode;
}

State GameState::state() const
{
	return p->m_state;
}

int GameState::points() const{
	return p->m_points;
}

void GameState::setMode(Mode mode){
	p->m_mode = mode;
}


void GameState::setState(State state){
	if (p->m_state == State::Finished) //cannot be changed (except with startNewGame slot)
		return;
	//set new state
	p->m_state = state;
}


void GameState::addPoints(int removedDiamonds){
	p->m_points += ++p->m_cascade;
}

void GameState::removePoints(int points){
	p->m_points = qMax(0, p->m_points - points);
}

void GameState::resetCascadeCounter(){
	p->m_cascade = 0;
}

void GameState::startNewGame(){
	p->m_points = 0;
	p->m_cascade = 0;
    p->m_state = State::Playing;
}


