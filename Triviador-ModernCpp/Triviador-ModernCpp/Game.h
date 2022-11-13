#pragma once
#include <iostream>
#include <vector>
#include "AbstractQuestion.h"
#include "Player.h"

class Game
{
public:

	Game(int id, std::string dateAndTime, std::vector<Player> players, std::vector<AbstractQuestion*> questions);

	//getters
	int GetId();
	std::string GetDateAndTime();
	std::vector<Player> GetPlayers();
	std::vector<AbstractQuestion*> GetQuestions();

	//setters
	void SetId(int id);
	void SetDateAndTime(std::string date);
	void SetPlayers(std::vector<Player> players);
	void SetQuestions(std::vector<AbstractQuestion*> questions);

private:

	int m_id;

	std::string m_dateAndTime;
	std::vector<Player> m_players;
	std::vector<AbstractQuestion*> m_questions;

	//Map m_map;
	//

};

