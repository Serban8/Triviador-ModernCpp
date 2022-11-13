#include "Player.h"

Player::Player(int id, int points, int wins, int losses, std::string username, std::string password, std::vector<Game> gamesPlayed) :
    m_id(id),
    m_points(points),
    m_numberOfWins(wins),
    m_numberOfLosses(losses),
    m_username(username),
    m_password(password),
    m_gamesPlayed(gamesPlayed)
{
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

std::vector<Game> Player::GetGamesPlayed()
{
    return m_gamesPlayed;
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

void Player::SetGamesPlayed(std::vector<Game> games)
{
    m_gamesPlayed = games;
}
