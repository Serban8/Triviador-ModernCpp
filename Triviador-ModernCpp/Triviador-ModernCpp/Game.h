#pragma once
#include <iostream>
#include <vector>
#include "AbstractQuestion.h"
#include "Player.h"

class Game
{
public:

	Game(std::vector<Player> players, std::vector<AbstractQuestion*> questions);

	//getters
	std::vector<Player> GetPlayers();
	std::vector<AbstractQuestion*> GetQuestions();

	//setters
	void SetQuestions(std::vector<AbstractQuestion*> questions);

	//methods
	void AddInactivePlayer(Player player);

private:

	int m_id;

	std::string m_dateAndTime;
	std::vector<Player> m_activePlayers;
	std::vector<Player> m_inactivePlayers;
	std::vector<AbstractQuestion*> m_questions;

	//Map m_map;
	//

};

