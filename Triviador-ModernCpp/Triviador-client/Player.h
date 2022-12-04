#pragma once
#include <string>
#include <array>

//also needed in client
class Player
{
public:
	enum class Advatage :uint8_t {
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
	void useAdvantage(Advatage advantage);

private:
	int m_points = 0;
	bool m_isActive = true;
	std::string m_username;

	//array of advantages - pair with advantage and status: used = true, not used = false
	std::array<std::pair<Advatage, bool>, 3> m_advantages =
	{ {
		{Advatage::FiftyFifty, false},
		{Advatage::ChooseAnswer, false},
		{Advatage::SuggestAnswer, false}
	} };
};