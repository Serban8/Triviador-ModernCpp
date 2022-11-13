#include "Player.h"

Player::Player(int id, int points, int wins, int losses, std::string username, std::string password) :
	m_id(id),
	m_points(points),
	m_numberOfWins(wins),
	m_numberOfLosses(losses),
	m_username(username),
	m_password(password)
{
	//empty
}

int Player::GetId()
{
	return m_id;
}

int Player::GetPoints()
{
	return m_points;
}

int Player::GetNumberOfWins()
{
	return m_numberOfWins;
}

int Player::GetNumberOfLosses()
{
	return m_numberOfLosses;
}

std::string Player::GetUsername()
{
	return m_username;
}

std::string Player::GetPassword()
{
	return m_password;
}


void Player::SetId(int id)
{
	m_id = id;
}

void Player::SetPoints(int points)
{
	m_points = points;
}

void Player::SetNumberOfWins(int wins)
{
	m_numberOfWins = wins;
}

void Player::SetNumberOfLosses(int losses)
{
	m_numberOfLosses = losses;
}

void Player::SetUsername(std::string username)
{
	m_username = username;
}

void Player::SetPassword(std::string password)
{
	m_password = password;
}
