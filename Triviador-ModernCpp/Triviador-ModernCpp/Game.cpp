#include "Game.h"
#include "QuestionGenerator.h"

Game::Game(std::vector<Player> players) :
	m_activePlayers(players),
	m_map(Map(players.size()))
{
	//getting questions directly from online database;
	//in future we need to change this to get them from our local database;
	QuestionGenerator qg;
	m_multipleChoiceQuestions = qg.GenerateMultipleChoiceQuestions(50);
	//for testing purposes 
	std::array<int, 3> numericIncorrectAnswers = { 2000,2001,2002 };
	std::array<float, 3> numericIncorrectAnswers2 = { 2.12, 1.05, 3.15 };
	NumberQuestion<int> nq("What was the year Max Verstappen won his first WDC ? ", "Sports", 2021, numericIncorrectAnswers);
	NumberQuestion<int> nq2("When did Romania join the UE ? ", "Politics", 2007, numericIncorrectAnswers);
	NumberQuestion<float> nq3("What is the value of PI ? ", "Math", 3.14, numericIncorrectAnswers2);

	m_numberQuestions.push_back(nq);
	m_numberQuestions.push_back(nq2);
	m_numberQuestions.push_back(nq3);
	for (auto& q : m_numberQuestions)
	{
		if (std::holds_alternative<NumberQuestion<int>>(q))
		{
			auto x = std::get<NumberQuestion<int>>(q);
			std::cout << x.GetQuestion() <<" "<< x.GetCorrectAnswer() << std::endl;

		}
		else
		{
			auto x2 = std::get<NumberQuestion<float>>(q);
			std::cout << x2.GetQuestion() << " " << x2.GetCorrectAnswer() << std::endl;
		}
		
	}
}


std::vector<Player> Game::GetPlayers()
{
	return m_activePlayers;
}

void Game::AddInactivePlayer(Player player)
{
	auto playerIt = std::find(m_activePlayers.begin(), m_activePlayers.end(), player);

	if (playerIt != m_activePlayers.end()) {
		m_activePlayers.erase(playerIt);
	}

	m_inactivePlayers.push_back(player);
}

void Game::PlayGame()
{
	for (const auto &player : m_activePlayers)
	{
		std::cout << "Welcome, " << player.GetUsername()<<", it's in the game!" << std::endl;
	}
	ChoosingBases();
}

void Game::ChoosingBases()
{
	std::cout << m_map<<std::endl;
	std::cout << "Question: \n\n";
	auto q = m_numberQuestions.back();
	m_numberQuestions.pop_back();

	//printing the question:
	if (std::holds_alternative<NumberQuestion<int>>(q))
	{
		auto qInt = std::get<NumberQuestion<int>>(q);
		qInt.PrintQuestion();
	}
	else {
		auto qFloat = std::get<NumberQuestion<float>>(q);
		qFloat.PrintQuestion();
	}
}