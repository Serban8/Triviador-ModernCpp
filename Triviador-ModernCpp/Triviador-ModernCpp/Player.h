#pragma once
#include <string>
#include <array>

class Player
{
public:
	enum class Advatage :uint8_t {
		FiftyFifty,
		ChooseAnswer,
		SuggestAnswer
	};

public:
	Player(std::string username);

	//getters
	int GetPoints();
	std::string GetUsername();
	bool IsActive();

	//setters
	void SetPoints(int points);
	void SetActive(bool isActive);

	//methods
	void useAdvantage(Advatage advantage);

	friend bool operator==(const Player& p1, const Player& p2);

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