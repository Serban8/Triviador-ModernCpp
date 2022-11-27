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
	m_numberQuestions = qg.GenerateNumberAnswerQuestions();

	//for testing purposes
	//for (auto& q : m_numberQuestions)
	//{
	//	if (std::holds_alternative<NumberQuestion<int>>(q))
	//	{
	//		auto x = std::get<NumberQuestion<int>>(q);
	//		std::cout << x.GetQuestion() << " " << x.GetCorrectAnswer() << std::endl;

	//	}
	//	else
	//	{
	//		auto x2 = std::get<NumberQuestion<float>>(q);
	//		std::cout << x2.GetQuestion() << " " << x2.GetCorrectAnswer() << std::endl;
	//	}
	//}
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
	for (const auto& player : m_activePlayers)
	{
		std::cout << "Welcome, " << player.GetUsername() << ", it's in the game!" << std::endl;
	}
	ChoosingBases();
	DistributeTerritories();
}

//todo: also return how fast was the response
template<typename T>
std::vector<std::pair<Player, T>> Game::GetNumberAnswers(std::vector<Player> players)
{
	T answer;
	std::vector<std::pair<Player, T>> answers;
	std::cout << "Please enter your answers:" << std::endl;
	for (auto& p : players)
	{
		std::cout << p.GetUsername() << ": ";
		std::cin >> answer;
		answers.push_back({ p, answer });
	}
	return answers;
}

//todo: sort by taking into account the response time if difference is the same
template<typename T>
std::vector<Player> Game::SortPlayersByAnswers(std::vector<Player> players, T correctAnswer)
{
	std::vector<Player> sortedPlayers;
	using playerAndAnswer = std::pair<Player, T>;
	std::vector<playerAndAnswer> answers = GetNumberAnswers<T>(players);
	std::sort(
		begin(answers),
		end(answers),
		[correctAnswer](const playerAndAnswer& p1, const playerAndAnswer& p2) {
			auto& [player1, answer1] = p1;
			auto& [player2, answer2] = p2;
			return (std::abs(correctAnswer - answer1)) < (std::abs(correctAnswer - answer2));
		});
	//testing
	std::cout << "\nCorrect answer was: " << correctAnswer << std::endl;
	std::cout << "LEADERBOARD:\n";
	//
	for (const auto& a : answers)
	{
		auto& [player, answer] = a;
		sortedPlayers.push_back(player);
		//testing
		std::cout << player.GetUsername() << " " << answer << std::endl;
		//
	}
	return sortedPlayers;
}

void Game::ChoosingBases()
{
	std::cout << "\nChoosing bases:\n\n";
	std::cout << m_map << std::endl;

	//printing the question:
	std::vector<Player> sortedPlayers = AskNumberQuestion(m_activePlayers);
	for (const auto& p : sortedPlayers)
	{
		std::cout << "Please, " << p.GetUsername() << ", choose the position of your base: " << std::endl << "> ";
		int line, col;
		std::cin >> line >> col;
		--line;
		--col;
		while (!m_map[{line, col}].GetOwner().GetUsername().empty() || !(line >= 0 && line < m_map.GetHeight() && col >= 0 && col < m_map.GetWidth()))
		{
			std::cout << "Invalid choice, please choose another position\n< ";
			std::cin >> line >> col;
			--line;
			--col;
		}
		m_map[{line, col}].SetOwner(p);
		m_map[{line, col}].SetScore(300);
		m_map[{line, col}].SetType(Region::Type::Base);
	}
	std::cout << m_map << std::endl;
}

void Game::DistributeTerritories()
{
	const uint8_t topPlayerNumberOfChoices = m_activePlayers.size() - 1;
	uint8_t territoriesLeft = (m_map.GetHeight() * m_map.GetWidth()) - m_activePlayers.size();

	std::cout << "Choosing Territories: \n";
	while (territoriesLeft != 0) {
		//print the map
		std::cout << m_map << std::endl;

		//ask the question and determine the leaderboard
		std::vector<Player> sortedPlayers = AskNumberQuestion(m_activePlayers);

		//the last player does not choose any territory
		sortedPlayers.pop_back();

		//selecting the territories
		if (territoriesLeft >= (topPlayerNumberOfChoices * (topPlayerNumberOfChoices + 1) / 2)) {
			uint8_t numOfChoices = topPlayerNumberOfChoices;
			for (const auto& player : sortedPlayers) {
				uint8_t numOfChoicesLeft = numOfChoices;
				--numOfChoices;
				while (numOfChoicesLeft != 0) {
					//asking for territory
					std::cout << "Please, " << player.GetUsername() << ", choose the position of a territory you want: " << std::endl << "> ";
					int line, col;
					std::cin >> line >> col;
					--line;
					--col;
					while (!m_map[{line, col}].GetOwner().GetUsername().empty() || !(line >= 0 && line < m_map.GetHeight() && col >= 0 && col < m_map.GetWidth()))
					{
						std::cout << "Invalid choice, please choose another position\n< ";
						std::cin >> line >> col;
						--line; --col;
					}
					m_map[{line, col}].SetOwner(player); //region is territory and has score set to 100 by default
					--numOfChoicesLeft;
					--territoriesLeft;
				}
			}
		}
		else {
			//if there are not enough territories left for a full round, we distribute them one by one in the order of the leaderboard
			for (size_t i = 0; i < sortedPlayers.size() && territoriesLeft > 0; ++i) {
				const auto& player = sortedPlayers[i];
				//asking for territory
				std::cout << "Please, " << player.GetUsername() << " choose the position of a territory you want: " << std::endl;
				int line, col;
				std::cout << player.GetUsername() << ": ";
				std::cin >> line >> col;
				--line;
				--col;
				while (!m_map[{line, col}].GetOwner().GetUsername().empty() || !(line >= 0 && line < m_map.GetHeight() && col >= 0 && col < m_map.GetWidth()))
				{
					std::cout << "Invalid choice, please choose another position" << std::endl;
					std::cin >> line >> col;
					--line; --col;
				}
				m_map[{line, col}].SetOwner(player); //region is territory and has score set to 100 by default
				--territoriesLeft;
			}
		}
	}
	//testing
	std::cout << m_map;
}

std::vector<Player> Game::AskNumberQuestion(std::vector<Player> players)
{
	std::vector<Player> sortedPlayers;
	//ask a question and return the top players
	auto q = m_numberQuestions.back();
	m_numberQuestions.pop_back();
	//printing the question:
	std::cout << "Please answer this question: \n\n";
	if (std::holds_alternative<NumberQuestion<int>>(q))
	{
		auto qInt = std::get<NumberQuestion<int>>(q);
		std::cout << qInt;
		sortedPlayers = SortPlayersByAnswers(players, qInt.GetCorrectAnswer());
	}
	else {
		auto qFloat = std::get<NumberQuestion<float>>(q);
		std::cout << qFloat;
		sortedPlayers = SortPlayersByAnswers(players, qFloat.GetCorrectAnswer());
	}

	return sortedPlayers;
}
