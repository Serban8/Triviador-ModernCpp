#include "Region.h"

Region::Region()
{
	m_type = Type::Territory;
	m_score = 100;
}

Region::Region(std::shared_ptr<Player> owner, Type type, int score) :
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

void Region::SetOwner(std::shared_ptr<Player> owner)
{
	m_owner = owner;
}

void Region::SetType(Type type)
{
	m_type = type;
}

int Region::GetScore() const
{
	return m_score;
}

std::shared_ptr<Player> Region::GetOwner() const
{
	return m_owner;
}

Region::Type Region::GetType() const
{
	return m_type;
}

void Region::UpdateRegion(const std::shared_ptr<Player> winner) {
	if (winner == m_owner) {
		m_score += 100;
	}
	else if (m_score > 100) {
		m_score -= 100;
	}
	else {
		m_owner = winner;
	}
}