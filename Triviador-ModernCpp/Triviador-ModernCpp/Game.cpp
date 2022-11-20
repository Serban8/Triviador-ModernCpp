#include "Game.h"

Game::Game(std::vector<Player> players, std::vector<AbstractQuestion*> questions) :
	
	m_activePlayers(players),
	m_questions(questions)
{
}


std::vector<Player> Game::GetPlayers()
{
	return m_activePlayers;
}

std::vector<AbstractQuestion*> Game::GetQuestions()
{
	return m_questions;
}

void Game::SetQuestions(std::vector<AbstractQuestion*> questions)
{
	m_questions = questions;
}

void Game::AddInactivePlayer(Player player)
{
	auto playerIt = std::find(m_activePlayers.begin(), m_activePlayers.end(), player);

	if (playerIt != m_activePlayers.end()) {
		m_activePlayers.erase(playerIt);
	}

	m_inactivePlayers.push_back(player);
}
