#include "Game.h"

Game::Game(int id, std::string dateAndTime, std::vector<Player> players, std::vector<AbstractQuestion*> questions) :
	m_id(id),
	m_dateAndTime(dateAndTime),
	m_players(players),
	m_questions(questions)
{
}

int Game::GetId()
{
	return m_id;
}

std::string Game::GetDateAndTime()
{
	return m_dateAndTime;
}

std::vector<Player> Game::GetPlayers()
{
	return m_players;
}

std::vector<AbstractQuestion*> Game::GetQuestions()
{
	return m_questions;
}

void Game::SetId(int id)
{
	m_id = id;
}

void Game::SetDateAndTime(std::string date)
{
	m_dateAndTime = date;
}

void Game::SetPlayers(std::vector<Player> players)
{
	m_players = players;
}

void Game::SetQuestions(std::vector<AbstractQuestion*> questions)
{
	m_questions = questions;
}
