#pragma once
#include<iostream>
#include<vector>
#include<string>
#include"Game.h"

class Player
{
public:

	Player(int id, int points, int wins, int losses, std::string username, std::string password, std::vector<Game> gamesPlayed);

	//getters
	int GetId();
	int GetPoints();
	int GetNumberOfWins();
	int GetNumberOfLosses();
	std::string GetUsername();
	std::vector<Game> GetGamesPlayed();

	//setters
	void SetId(int id);
	void SetPoints(int points);
	void SetNumberOfWins(int wins);
	void SetNumberOfLosses(int losses);
	void SetUsername(std::string username);
	void SetPassword(std::string password);
	void SetGamesPlayed(std::vector<Game> games);

private:

	int m_id;
	int m_points;
	int m_numberOfWins;
	int m_numberOfLosses;

	std::string m_username;
	std::string m_password;
	
	std::vector<Game> m_gamesPlayed;

};

