#include "Player.h"

Player::Player(std::string username):
	m_username(username)
{
}

int Player::GetPoints()
{
	return m_points;
}

std::string Player::GetUsername()
{
	return m_username;
}

bool Player::IsActive()
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
