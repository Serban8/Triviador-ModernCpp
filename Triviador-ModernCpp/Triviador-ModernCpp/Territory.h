#pragma once

#include<string>
#include"Player.h"

class Territory
{
public:

	//constructor
	Territory(Player owner, int totalScore = 0);


	//setters
	void SetScore(int score);

	//getters
	int GetTotalScore();
	Player GetOwner();

	//methods

private:

	int m_score;
	Player m_owner;

};

