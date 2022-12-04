#pragma once
#include <string>
#include <vector>
#include "Game.h"
#include"Player.h"
#include <ctime>
struct GameDatabase
{
	GameDatabase() {} 

	//creating a struct as a connection between object in game and object in database
	GameDatabase(std::string winner, int rounds) :
		//will initialize with the player that wins the game, ideally from the database table
		//m_winner(game.winner().GetUsername())
		m_winner(winner),
		m_rounds(rounds)
	{
		//initialize time
		time_t now = time(0);
		tm ltm; 
		localtime_s(&ltm,&now);
		//add year
		m_date.append(std::to_string(1900 + ltm.tm_year));
		m_date.append("-");
		//add month
		m_date.append(std::to_string(1 + ltm.tm_mon));
		m_date.append("-");
		//add day
		m_date.append(std::to_string(ltm.tm_mday));
	
	}

	int m_id;
	int m_rounds;
	std::string m_winner;
	std::string m_date;

};
