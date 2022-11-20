#pragma once
#include <iostream>
#include <vector>
#include "MultipleChoiceQuestion.h"
#include "NumberQuestion.h"
#include "Player.h"
#include "Region.h"

class Game
{
public:

	Game(std::vector<Player> players);

	//getters
	std::vector<Player> GetPlayers();

	//methods
	void AddInactivePlayer(Player player);

private:

	std::vector<Player> m_activePlayers;
	std::vector<Player> m_inactivePlayers;
	//std::vector<NumberQuestion> m_numberQuestions;
	std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions;
	std::vector<std::vector<Region>> m_map;

};

