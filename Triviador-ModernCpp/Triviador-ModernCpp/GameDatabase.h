#pragma once
#include <string>
#include <ctime>

#include "Player.h"

///
/// Keeping file in case a need for specific functions appears
/// TO DELETE OTHERWISE
///

//struct GameDatabase
//{
//	GameDatabase() = default;
//
//	//winner is a FK to players table
//	GameDatabase(std::string winner, int rounds) :
//		m_winner(winner),
//		m_rounds(rounds)
//	{
//		//initialize time
//		time_t now = time(0);
//		tm ltm;
//		localtime_s(&ltm, &now);
//		//add year
//		m_date.append(std::to_string(1900 + ltm.tm_year));
//		m_date.append("-");
//		//add month
//		m_date.append(std::to_string(1 + ltm.tm_mon));
//		m_date.append("-");
//		//add day
//		m_date.append(std::to_string(ltm.tm_mday));
//	}
//
//	int m_id;
//	int m_rounds;
//	std::string m_winner;
//	std::string m_date;
//
//};
