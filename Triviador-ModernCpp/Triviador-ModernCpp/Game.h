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
	template<typename T> std::vector<T> getAnswers(std::vector<Player> players);
private:

	void ChoosingBases();

private:

	std::vector<Player> m_activePlayers;
	std::vector<Player> m_inactivePlayers;
	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> m_numberQuestions;
	std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions;
	Map m_map;

};

template<typename T> std::vector<T> Game::getAnswers(std::vector<Player> players)
{
	T answer;
	std::vector<T> answers;
	std::cout << "Please enter your answers:" << std::endl;
	for (auto& p : players)
	{
		std::cout << p.GetUsername() << ": ";
		std::cin >> answer;
		answers.push_back(answer);
	}
	return answers;
}

