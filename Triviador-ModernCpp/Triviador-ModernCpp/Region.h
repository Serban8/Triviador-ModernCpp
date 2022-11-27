#pragma once

#include <string>
#include "Player.h"

class Region
{
public:
	enum class Type : uint8_t {
		Base,
		Territory
	};

public:
	//constructor
	Region();
	Region(Player owner, Type type = Type::Territory, int score = 100);

	//setters
	void SetScore(int score);
	void SetOwner(Player owner);

	//getters
	int GetScore() const;
	Player GetOwner() const;
	Type GetType() const;

	//methods
	//

private:
	static const int s_baseInitialScore = 300;
	static const int s_territoryInitialScore = 100;
private:
	int m_score;
	Player m_owner;
	Type m_type;
};

