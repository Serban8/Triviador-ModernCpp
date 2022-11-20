#include "Game.h"
#include "QuestionGenerator.h"

Game::Game(std::vector<Player> players) :
	m_activePlayers(players)
	
{
	//getting questions directly from online database;
	//in future we need to change this to get them from our local database;
	QuestionGenerator qg;
	m_multipleChoiceQuestions = qg.GenerateMultipleChoiceQuestions(50);
	//m_numberQuestions = qg.GenerateNumberQuestions(50);
	switch (players.size()) {
	case 2:
		m_map.resize(3);
		for (auto& line : m_map) {
			line.resize(3);
		}
		break;
	case 3:
		m_map.resize(5);
		for (auto& line : m_map) {
			line.resize(3);
		}
		break;
	case 4:
		m_map.resize(6);
		for (auto& line : m_map) {
			line.resize(4);
		}
		break;
	default:
		throw std::runtime_error("Number of players has to be between 2 and 4.");
	}
}


std::vector<Player> Game::GetPlayers()
{
	return m_activePlayers;
}

void Game::AddInactivePlayer(Player player)
{
	auto playerIt = std::find(m_activePlayers.begin(), m_activePlayers.end(), player);

	if (playerIt != m_activePlayers.end()) {
		m_activePlayers.erase(playerIt);
	}

	m_inactivePlayers.push_back(player);
}
