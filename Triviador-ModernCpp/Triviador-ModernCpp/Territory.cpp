#include "Territory.h"

Territory::Territory(Player owner, int totalScore) :
	m_owner(owner),
	m_score(totalScore)
{
}

void Territory::SetScore(int score)
{
	m_score = score;
}

int Territory::GetTotalScore()
{
	return m_score;
}

Player Territory::GetOwner()
{
	return m_owner;
}
