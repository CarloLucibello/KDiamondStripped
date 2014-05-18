#include "gamestate.h"

#include <iostream> //DEBUG
using namespace std;

GameState::GameState() {}



Mode GameState::mode() const{
    return m_mode;
}

State GameState::state() const
{
    return m_state;
}

int GameState::points() const{
    return m_points;
}

void GameState::setMode(Mode mode){
    m_mode = mode;
}


void GameState::setState(State state){
    if (m_state == State::Finished) //cannot be changed (except with startNewGame slot)
		return;
	//set new state
    m_state = state;
}

void GameState::addPoints(int points){
    m_points += points;
}


void GameState::updateMovesLeft(int n){
    m_movesLeft -= n;
}

int GameState::movesLeft(){
    return m_movesLeft;
}

void GameState::removePoints(int points){
    m_points = max(0, m_points - points);
}


void GameState::startNewGame(const GameParams * gameParams){
    m_points = 0;
    m_state = State::Playing;
    m_movesLeft = gameParams->moves();
}


