#include "Player.h"

Player::Player(std::string username):
	m_username(username)
{
}

int Player::GetPoints() const
{
	return m_points;
}

std::string Player::GetUsername() const
{
	return m_username;
}

bool Player::IsActive() const
{
	return m_isActive;
}

void Player::SetPoints(int points)
{
	m_points = points;
}

void Player::SetActive(bool isActive)
{
	m_isActive = isActive;
}

void Player::useAdvantage(Advatage advantage)
{
	m_advantages.at(static_cast<uint8_t>(advantage)).second = true;
}

bool operator==(const Player& p1, const Player& p2)
{
	return p1.m_username == p2.m_username;
}
