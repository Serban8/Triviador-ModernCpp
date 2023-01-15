#pragma once
#include <iostream>
#include <vector>
#include <variant>

#include "..\Triviador-DLL\MultipleChoiceQuestion.h"
#include "..\Triviador-DLL\NumberQuestion.h"
#include "Player.h"
#include "Map.h"

using NumberQ = std::variant<NumberQuestion<int>, NumberQuestion<float>>;

class Game
{
public:
	enum class Phase : uint8_t {
		CHOOSING_BASES,
		CHOOSING_REGIONS,
		DUEL
	};
public:
	Game() = default;
	Game(std::vector<Player>& players, std::vector<NumberQ> m_numberQuestions, std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions);

	//operators
	//getter - takes an username as param and returns the constant reference to it
	const Player& operator[] (const std::string& pos) const;
	//getter and/or setter - return value is non-const; any changes made outside of this class are reflected inside
	Player& operator[] (const std::string& pos);

	//getters
	std::vector<Player> GetActivePlayers() const;
	size_t GetNumberOfActivePlayers() const;
	size_t GetNumberOfPlayers() const;

	NumberQ GetNumberQuestion();
	MultipleChoiceQuestion GetMultipleChoiceQuestion();

	Map GetMap() const;
	std::shared_ptr<Region> GetRegion(Map::Position regionIndex);

	Phase GetPhase() const;

	//setters
	void SetPhase(Phase phase);

	void ModifyRegion(const Map::Position pos, std::shared_ptr<Player> newOwner);
	void ModifyRegion(const Map::Position pos, Region::Type newType, std::shared_ptr<Player> newOwner);

	void IncreaseRegionScore(const Map::Position pos);
	void DecreaseRegionScore(const Map::Position pos);

	//methods
	void AddInactivePlayer(const std::string player);

private:
	std::unordered_map<std::string, std::shared_ptr<Player>> m_playersMap;
private:
	std::vector<Player> m_activePlayers;
	std::vector<Player> m_inactivePlayers;
	std::vector<NumberQ> m_numberQuestions;
	std::vector<MultipleChoiceQuestion> m_multipleChoiceQuestions;
	Map m_map;
	Phase m_phase;
};