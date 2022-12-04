#pragma once
#include <string>
#include <array>

//also needed in client
class Player
{
public:
	enum class Advantage :uint8_t {
		FiftyFifty,
		ChooseAnswer,
		SuggestAnswer
	};

public:
	Player() = default;
	Player(std::string username);

	//operators
	friend bool operator==(const Player& p1, const Player& p2);
	friend bool operator<(const Player& p1, const Player& p2);

	//getters
	int GetPoints() const;
	std::string GetUsername() const;
	bool IsActive() const;

	//setters
	void SetPoints(int points);
	void SetActive(bool isActive);

	//methods
	void UseAdvantage(Advantage advantage);

private:
	int m_points = 0;
	bool m_isActive = true;
	std::string m_username;

	//array of advantages - pair with advantage and status: used = true, not used = false
	std::array<std::pair<Advantage, bool>, 3> m_advantages =
	{ {
		{Advantage::FiftyFifty, false},
		{Advantage::ChooseAnswer, false},
		{Advantage::SuggestAnswer, false}
	} };
};