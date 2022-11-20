#include "Region.h"

Region::Region(Player owner, Type type, int score) :
	m_owner(owner),
	m_type(type)
{
	if (type == Type::Territory) {
		m_score = s_territoryInitialScore;
	}
	else {
		m_score = s_baseInitialScore;
	}
}

void Region::SetScore(int score)
{
	m_score = score;
}

void Region::SetOwner(Player owner)
{
	m_owner = owner;
}

int Region::GetScore()
{
	return m_score;
}

Player Region::GetOwner()
{
	return m_owner;
}

Region::Type Region::GetType()
{
	return m_type;
}
