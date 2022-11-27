#pragma once
#include <iostream>
#include <vector>
#include <variant>

#include "MultipleChoiceQuestion.h"
#include "NumberQuestion.h"
#include "Player.h"
#include "Map.h"

class Game
{
public:

	Game(std::vector<Player> players);

	//getters
	std::vector<Player> GetPlayers();

	//methods
	void AddInactivePlayer(Player player);
	void PlayGame();

private:
	template<typename T>
	std::vector<std::pair<Player, T>> GetNumberAnswers(std::vector<Player> players);
	template<typename T>
	std::vector<Player> SortPlayersByAnswers(std::vector<Player> players, T correctAnswer);
	void ChoosingBases();
private:

	std::vector<Player> m_activePlayers;
	std::vector<Player> m_inactivePlayers;
	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> m_numberQuestions;
	std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions;
	Map m_map;

};



