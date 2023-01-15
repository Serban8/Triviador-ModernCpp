#include "Game.h"
#include "QuestionGenerator.h"

Game::Game(std::vector<Player>& players, std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> numberQuestions, std::vector<MultipleChoiceQuestion> multipleChoiceQuestions) :
	m_activePlayers(players),
	m_map(Map(players.size())),
	m_multipleChoiceQuestions(multipleChoiceQuestions),
	m_numberQuestions(numberQuestions)
{
	for (const Player& player : m_activePlayers) {
		m_playersMap.emplace(player.GetUsername(), std::make_shared<Player>(player));
	}
	m_phase = Phase::CHOOSING_BASES;
}

const Player& Game::operator[](const std::string& username) const
{
	return *m_playersMap.at(username).get();
}

Player& Game::operator[](const std::string& username)
{
	return *m_playersMap.at(username).get();
}

std::vector<Player> Game::GetActivePlayers() const
{
	return m_activePlayers;
}

size_t Game::GetNumberOfActivePlayers() const
{
	return m_activePlayers.size();
}

size_t Game::GetNumberOfPlayers() const
{
	return m_playersMap.size();
}

void Game::AddInactivePlayer(const std::string player)
{
	auto playerIt = std::find(m_activePlayers.begin(), m_activePlayers.end(), player);

	if (playerIt != m_activePlayers.end()) {
		m_inactivePlayers.push_back(std::move(*playerIt));
	}
}

std::variant<NumberQuestion<int>, NumberQuestion<float>> Game::GetNumberQuestion()
{
	std::variant<NumberQuestion<int>, NumberQuestion<float>> q;
	if (m_numberQuestions.size() > 0) {
		q = m_numberQuestions.back();
		m_numberQuestions.pop_back();
	}
	return q;
}

MultipleChoiceQuestion Game::GetMultipleChoiceQuestion()
{
	if (m_multipleChoiceQuestions.size() > 0) {
		MultipleChoiceQuestion q = m_multipleChoiceQuestions.back();
		m_multipleChoiceQuestions.pop_back();
		return q;
	}
	else {
		//return empty question
		return MultipleChoiceQuestion("", "", "", std::array<std::string, 3>{"", "", ""});
	}
}

Map Game::GetMap() const
{
	return m_map;
}

std::shared_ptr<Region> Game::GetRegion(Map::Position regionIndex)
{
	return std::make_shared<Region>(m_map[regionIndex]);
}

Game::Phase Game::GetPhase() const
{
	return m_phase;
}

void Game::SetPhase(Phase phase)
{
	m_phase = phase;
}

void Game::ModifyRegion(const Map::Position pos, std::shared_ptr<Player> newOwner)
{
	m_map[pos].SetOwner(newOwner);
}

void Game::ModifyRegion(const Map::Position pos, Region::Type newType, std::shared_ptr<Player> newOwner)
{
	m_map[pos].SetType(newType);
	m_map[pos].SetOwner(newOwner);
}

void Game::IncreaseRegionScore(const Map::Position pos)
{
	const int increaseAmount = 100;
	m_map[pos].SetScore(m_map[pos].GetScore() + increaseAmount);
}

void Game::DecreaseRegionScore(const Map::Position pos)
{
	const int decreaseAmount = 100;
	m_map[pos].SetScore(m_map[pos].GetScore() - decreaseAmount);
}
