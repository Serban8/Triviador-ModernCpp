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
	Game() = default;
	Game(std::vector<Player>& players, std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> m_numberQuestions,std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions);

	//operators
	//getter - takes an username as param and returns the constant reference to it
	const Player& operator[] (const std::string& pos) const;
	//getter and/or setter - return value is non-const; any changes made outside of this class are reflected inside
	Player& operator[] (const std::string& pos);

	//getters
	std::vector<Player> GetActivePlayers() const;
	size_t GetNumberOfActivePlayers() const;
	size_t GetNumberOfPlayers() const;
	std::variant<NumberQuestion<int>, NumberQuestion<float>> GetNumberQuestion();
	MultipleChoiceQuestion GetMultipleChoiceQuestion();
	Map GetMap() const;

	//setters
	void ModifyRegion(const Map::Position pos, Region::Type newType); //possibly not needed
	void ModifyRegion(const Map::Position pos, Player newOwner);
	void ModifyRegion(const Map::Position pos, Region::Type newType, Player newOwner);

	void IncreaseRegionScore(const Map::Position pos);
	void DecreaseRegionScore(const Map::Position pos);

	//methods
	void AddInactivePlayer(const std::string player);
	void PlayGame();
	//Here for testing only
	void printPlayerMap();
	std::vector<Player> DetermineWinners();
	//

private:
	//the main steps of the game
	void ChoosingBases();
	void DistributeTerritories();

	//helpers
	std::vector<Player> AskNumberQuestion(std::vector<Player> players); //asks the players given as parameter a guestion and returns them in a sorted vector (winner is first)
	template<typename T>
	std::vector<std::pair<Player, T>> GetNumberAnswers(std::vector<Player> players) const; //asks the players given as parameter to input an answer
	template<typename T>
	std::vector<Player> SortPlayersByAnswers(std::vector<Player> players, T correctAnswer) const; //gets the answers from players and then sorts them
private:
	std::unordered_map<std::string, std::unique_ptr<Player>> m_playersMap;
private:
	std::vector<Player> m_activePlayers;
	std::vector<Player> m_inactivePlayers;
	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> m_numberQuestions;
	std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions;
	Map m_map;
};