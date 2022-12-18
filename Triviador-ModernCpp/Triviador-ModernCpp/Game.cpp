#include "Game.h"
#include "QuestionGenerator.h"

Game::Game(std::vector<Player>& players) :
	m_activePlayers(players),
	m_map(Map(players.size()))
{
	//the game is initialized with a vector of players. We should have a function which handles player login - finding players
	//
	//getting questions directly from online database;
	//in future we need to change this to get them from our database;
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


std::vector<Player> Game::GetPlayers() const
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
	//TOOO: display starting animation/message to all players (in client)
	for (const auto& player : m_activePlayers)
	{
		std::cout << "Welcome, " << player.GetUsername() << ". It's in the game." << std::endl;
	}
	ChoosingBases();
	DistributeTerritories();
}

//todo: also return how fast was the response
template<typename T>
std::vector<std::pair<Player, T>> Game::GetNumberAnswers(std::vector<Player> players) const
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
std::vector<Player> Game::SortPlayersByAnswers(std::vector<Player> players, T correctAnswer) const
{
	//Setp 3 - Sorting the players according to their response
	using playerAndAnswer = std::pair<Player, T>;

	std::vector<Player> sortedPlayers;

	//todo: the function should recieve the answers as parameter
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
	//1. Get question from vector
	//2. Display question & ask for answers
	//3. Create leaderboard
	//4. Get players to choose bases according to leaderboard
	//5. Update map
	std::cout << "\nChoosing bases:\n\n";
	std::cout << m_map << std::endl;

	//AskNumberQuestion handles setps 1-3
	std::vector<Player> sortedPlayers = AskNumberQuestion(m_activePlayers);

	//Setp 4. Choosing bases - (in client) each player should be promted to choose base and while a player is choosing the others should be informed about that
	//						 - (in server) we should iterate through the list of players and update with the info received from the client
	for (const auto& p : sortedPlayers)
	{
		std::cout << "Please, " << p.GetUsername() << ", choose the position of your base: " << std::endl << "> ";
		int line, col;
		std::cin >> line >> col;
		--line; --col; //player input pos between (1, height) and (1, width)

		//input validation
		while (!(line >= 0 && line < m_map.GetHeight() && col >= 0 && col < m_map.GetWidth()) || !m_map[{line, col}].GetOwner().GetUsername().empty())
		{
			std::cout << "Invalid choice, please choose another position\n> ";
			std::cin >> line >> col;
			--line; --col;
		}
		//Step 5 - map update should be done here and reflected in client
		m_map[{line, col}].SetOwner(p);
		m_map[{line, col}].SetScore(300);
		m_map[{line, col}].SetType(Region::Type::Base);
	}
	std::cout << m_map << std::endl;
}

void Game::DistributeTerritories()
{
	//1. Get question from vector
	//2. Display question & ask for answers
	//3. Create leaderboard
	//4. Get players to choose territories according to leaderboard and rules
	//5. Update map

	const uint8_t topPlayerNumberOfChoices = m_activePlayers.size() - 1;
	//the territories left undistributed are all except the bases
	uint8_t territoriesLeft = (m_map.GetHeight() * m_map.GetWidth()) - m_activePlayers.size();

	std::cout << "Choosing Territories: \n";
	while (territoriesLeft != 0) {
		//print the map (client)
		std::cout << m_map << std::endl;

		//ask the question and determine the leaderboard
		//AskNumberQuestion handles setps 1-3
		std::vector<Player> sortedPlayers = AskNumberQuestion(m_activePlayers);

		//the last player does not choose any territory
		sortedPlayers.pop_back();

		//Setp 4 - selecting territories
		//Each player selects a set amount of territories. This should be handled by client
		//    server sends the player that should select and the number of territories they should select
		//    the client returns the chosen territories (the client handles validation)
		if (territoriesLeft >= (topPlayerNumberOfChoices * (topPlayerNumberOfChoices + 1) / 2)) {
			//gauss sum represents the number of territories distributed in a normal round
			uint8_t numOfChoices = topPlayerNumberOfChoices;
			for (const auto& player : sortedPlayers) {
				//here - send needed info to client and parse response
				uint8_t numOfChoicesLeft = numOfChoices;
				--numOfChoices;
				while (numOfChoicesLeft != 0) {
					//asking for territory
					std::cout << "Please, " << player.GetUsername() << ", choose the position of a territory you want: " << std::endl << "> ";
					int line, col;
					std::cin >> line >> col;
					--line; --col;
					while (!(line >= 0 && line < m_map.GetHeight() && col >= 0 && col < m_map.GetWidth()) || !m_map[{line, col}].GetOwner().GetUsername().empty())
					{
						std::cout << "Invalid choice, please choose another position\n> ";
						std::cin >> line >> col;
						--line; --col;
					}
					//Step 5 - update the map and sync it with client map
					m_map[{line, col}].SetOwner(player); //region is territory and has score set to 100 by default
					--numOfChoicesLeft;
					--territoriesLeft;
				}
			}
		}
		else {
			//if there are not enough territories left for a full round, we distribute them one by one in the order of the leaderboard
			for (size_t i = 0; i < sortedPlayers.size() && territoriesLeft > 0; ++i) {
				//here - send needed info to client and parse response
				const auto& player = sortedPlayers[i];
				//asking for territory
				std::cout << "Please, " << player.GetUsername() << " choose the position of a territory you want: " << std::endl;
				int line, col;
				std::cout << player.GetUsername() << ": ";
				std::cin >> line >> col;
				--line; --col;
				while (!(line >= 0 && line < m_map.GetHeight() && col >= 0 && col < m_map.GetWidth()) || !m_map[{line, col}].GetOwner().GetUsername().empty())
				{
					std::cout << "Invalid choice, please choose another position" << std::endl;
					std::cin >> line >> col;
					--line; --col;
				}
				//Step 5 - update the map and sync it with client map
				m_map[{line, col}].SetOwner(player); //region is territory and has score set to 100 by default
				--territoriesLeft;
			}
		}
	}
	//testing
	std::cout << m_map;
}
std::variant<NumberQuestion<int>, NumberQuestion<float>> Game::GetNumberQuestion()
{
	std::variant<NumberQuestion<int>, NumberQuestion<float>> q;
	if (m_numberQuestions.size() > 0) {
		q = m_numberQuestions.back();
		m_numberQuestions.pop_back();
	}
	else {
		//to do: return empty variant with std::monostate
		return q;
	}
	return q;
}
std::vector<Player> Game::AskNumberQuestion(std::vector<Player> players)
{
	//1. Get question from vector
	//2. 2.1 Display question & 
	//	 2.2 Ask for answers
	//3. Create leaderboard

	//Step 1 - Getting the question
	std::vector<Player> sortedPlayers;
	auto q = m_numberQuestions.back();
	m_numberQuestions.pop_back();

	//Step 2 -> Displaying the question (should be handles in client - pack the info and sending it to each client)
	//		 -> Each client should return the answer of each player
	//
	//Setp 3 is handles in SortPLayersByAnswers
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

//Should also send the leaderboard to the client
std::vector<Player> Game::DetermineWinners()
{
	std::vector<Player> winners;
	std::vector<Player> losers = m_inactivePlayers; //for database update

	if (m_activePlayers.size() == 1)
		return m_activePlayers;

	auto maxPoints = std::max_element(m_activePlayers.begin(), m_activePlayers.end());
	Player playerMaxPoints;

	//check if there are more than one player with max score
	do {
		playerMaxPoints = *maxPoints;
		winners.push_back(playerMaxPoints);
		m_activePlayers.erase(maxPoints);
		maxPoints = std::find_if(m_activePlayers.begin(), m_activePlayers.end(),
			[&playerMaxPoints](const Player& p2) {
				return playerMaxPoints.GetPoints() == p2.GetPoints();
			});
	} while (maxPoints != m_activePlayers.end());

	losers.insert(losers.end(), m_activePlayers.begin(), m_activePlayers.end());
	return winners;
}