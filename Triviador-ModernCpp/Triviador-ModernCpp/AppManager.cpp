#include "AppManager.h"

AppManager::AppManager(crow::SimpleApp& app, Storage& storage) : m_app(app), m_storage(storage)
{
	m_status = ServerStatus::WAIT_TO_START;

	//status check
	CROW_ROUTE(app, "/checkstatus")
		.methods(crow::HTTPMethod::PUT)([this](const crow::request& req) {
		auto bodyArgs = parseRequestBody(req.body);
		auto usernameIter = bodyArgs.find("username");

		if (usernameIter != bodyArgs.end() && m_status != ServerStatus::WAIT_TO_START)
		{
			std::string username = usernameIter->second;
			Player requestingPlayer = m_game[username];
			//

			//COMMON LOGIC -> to add DISPLAY_LEADERBARD here leaderboard as well
			if (m_status == ServerStatus::DISPLAY_QUESTION) {
				//send the DISPLAY_QUESTION signal only to the players that have not already answered
				//if we can find the player in the leaderboard then he already answered and needs to be sent the status to wait
				auto it = std::find_if(
					m_leaderboard.begin(),
					m_leaderboard.end(),
					[&username](const auto& val) { return val.second->GetUsername() == username; }
				);

				if (it != m_leaderboard.end()) {
					return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::WAIT) } };
				}

				return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::DISPLAY_QUESTION) } };

			}
			//

			//CHOOSING BASES PHASE LOGIC
			else if (m_game.GetPhase() == Game::Phase::CHOOSING_BASES) {
				if (m_status == ServerStatus::WAIT) {
					if (m_leaderboard.empty()) { //choosing bases is done - start next phase
						m_game.SetPhase(Game::Phase::CHOOSING_REGIONS);
						m_status = ServerStatus::START_CHOOSING_REGIONS;
						CROW_LOG_INFO << "---";
						CROW_LOG_INFO << "Status has been set to START_CHOOSING_REGIONS";
						CROW_LOG_INFO << "---";
					}
					else {
						//if it is the requesting players' turn to choose the base then return CHOOSE_REGION and erase him from the leaderboard
						const Player intendedChoosingPlayer = *m_leaderboard.begin()->second.get();
						if (requestingPlayer == intendedChoosingPlayer) {
							return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::CHOOSE_REGION) } };
						}
					}
				}
				else if (m_status == ServerStatus::CHOSE_BASE) {//check if needed
					m_status = ServerStatus::UPDATE_MAP;
					m_leaderboard.erase(m_leaderboard.begin());
				}
				return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
			}
			//

			//CHOOSING TERRITORIES PHASE LOGIC
			//if current phase is set to choosing territories, then send the players (in the correct order) the signal to choose the territory
			else if (m_game.GetPhase() == Game::Phase::CHOOSING_REGIONS) {
				static int numberOfChoicesAllowed;

				if (m_status == ServerStatus::START_CHOOSING_REGIONS) {
					//make sure that all clients have received START_CHOOSING_REGIONS to start choosing regions
					static uint8_t requestCount = 0;
					++requestCount;
					if (requestCount == m_game.GetActivePlayers().size()) {
						CROW_LOG_INFO << "---";
						CROW_LOG_INFO << "Status has been set to GET_NUMBER_QUESTION";
						CROW_LOG_INFO << "---";
						m_status = ServerStatus::GET_NUMBER_QUESTION;
						numberOfChoicesAllowed = m_game.GetActivePlayers().size() - 1;
						requestCount = 0;
					}
					return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::WAIT) } };
				}
				else if (m_status == ServerStatus::WAIT || m_status == ServerStatus::UPDATE_MAP) {

					if (m_leaderboard.empty()) { //this loop of choosing regions is done
						if (m_game.GetMap().AreAllRegionsOwned()) {
							m_game.SetPhase(Game::Phase::DUEL);
							CROW_LOG_INFO << "---";
							CROW_LOG_INFO << "Status has been set to START_ROUND";
							CROW_LOG_INFO << "---";
							GenereatePlayerOrder(); //generate the player order for the DUEL phase and start the first roud
							m_status = ServerStatus::START_ROUND;
						}
						else {
							CROW_LOG_INFO << "---";
							CROW_LOG_INFO << "Status has been set to START_CHOOSING_REGIONS";
							CROW_LOG_INFO << "---";
							m_status = ServerStatus::START_CHOOSING_REGIONS;
						}
						return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
					}
					else {
						static int numberChoseCounter = 0;
						//if it is the requesting players' turn to choose the base then update the counter
						const Player intendedChoosingPlayer = *m_leaderboard.begin()->second.get();
						if (requestingPlayer == intendedChoosingPlayer) {
							//check if the player is done choosing
							if (numberChoseCounter == numberOfChoicesAllowed) {
								--numberOfChoicesAllowed;
								numberChoseCounter = 0;
								m_leaderboard.erase(m_leaderboard.begin());
							}
							//check if the player still has to choose
							else if (numberChoseCounter < numberOfChoicesAllowed) {
								++numberChoseCounter;
								return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::CHOOSE_REGION) } };
							}
						}
					}
					return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
				}
				else if (m_status == ServerStatus::CHOSE_REGION) {
					m_status = ServerStatus::UPDATE_MAP;
					return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
				}
				return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
			}
			//

			//DUEL PHASE LOGIC
			//on START_ROUND
			else if (m_game.GetPhase() == Game::Phase::DUEL) {
				if (m_status == ServerStatus::START_ROUND) {
					//get the intended attacking player and check if the request is made from him
					//if playerOrder queue is empty then the game has finished
					if (m_playerOrder.empty()) {
						CROW_LOG_INFO << "---";
						CROW_LOG_INFO << "Status has been set to GAME_FINISHED";
						CROW_LOG_INFO << "---";
						m_status = ServerStatus::GAME_FINISHED;
						return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
					}
					const Player intendedAttackingPlayer = m_game[m_playerOrder.front()];
					if (requestingPlayer == intendedAttackingPlayer) {
						m_attacker = m_game[username]; //should be pointer
						CROW_LOG_INFO << "---";
						CROW_LOG_INFO << "Status has been set to WAIT";
						CROW_LOG_INFO << "---";

						//only return to the attacker CHOOSE_REGION signal and set the status to WAIT for all other players
						m_status = ServerStatus::WAIT;
						return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::CHOOSE_REGION) } };
					}
					return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };

				}

				//on ATTACKER_CHOSE_REGION
				else if (m_status == ServerStatus::ATTACKER_CHOSE_REGION) {
					static uint8_t requestsCounter = 0;
					m_playerOrder.pop();
					//send the attacker and the attacked the signal to ask for question
					if (requestingPlayer == m_attacker || requestingPlayer == m_attacked) {
						++requestsCounter;
						if (requestsCounter == 2) {
							CROW_LOG_INFO << "---";
							CROW_LOG_INFO << "Status has been set to DISPLAY_QUESTION";
							CROW_LOG_INFO << "---";
							m_status = ServerStatus::DISPLAY_QUESTION;
							requestsCounter = 0;
						}
						return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::GET_MULTIPLE_CHOICE_QUESTION) } };
					}
					else {
						return crow::json::wvalue{ { "status", ConvertStatusToString(ServerStatus::WAIT) } };
					}
				}
			}

			//ON UPDATE_MAP - everybody gets the map
			//

			else {
				return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } }; //automatically sends code 500 error if exception is thrown 
			}
		}
		else if (m_status == ServerStatus::WAIT_TO_START || m_status == ServerStatus::START_CHOOSING_REGIONS) {
			return crow::json::wvalue{ { "status", ConvertStatusToString(m_status) } };
		}

			});
	//

	//login related routes
	CROW_ROUTE(app, "/addnewplayer") //move to functor
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::addNewPlayer(storage, req);
			});
	CROW_ROUTE(app, "/checkplayer") //move to functor
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::checkPlayer(storage, req);
			});
	//

	//waiting room related routes
	CROW_ROUTE(app, "/addtowaitingroom")
		.methods(crow::HTTPMethod::PUT)(AddToWaitingRoomHandler(m_waitingRoomList, m_storage));
	CROW_ROUTE(app, "/checkwaitingroom")(CheckWaitingRoomHandler(m_waitingRoomList, m_votesToStart));
	CROW_ROUTE(app, "/addvote")
		.methods(crow::HTTPMethod::PUT)(AddVoteHandler(m_waitingRoomList, m_votesToStart));
	//

	//game history
	CROW_ROUTE(app, "/player/<string>")(PlayerHandler(m_storage));
	//

	//game logic related routes
	CROW_ROUTE(app, "/startgame")(StartGameHandler(m_status, m_game, m_waitingRoomList, storage));
	CROW_ROUTE(app, "/getplayers")(GetPlayersHandler(m_game));
	CROW_ROUTE(app, "/getnumberquestion")(GetNumberQuestionHandler(m_status, m_game));
	CROW_ROUTE(app, "/getmultiplechoicequestion")
		.methods(crow::HTTPMethod::PUT)(GetMultipleChoiceQuestionHandler(m_status, m_attacker, m_attacked, m_game));
	//map related routes
	CROW_ROUTE(app, "/getmap")(GetMapHandler(m_status, m_game));
	CROW_ROUTE(app, "/setregionowner")
		.methods(crow::HTTPMethod::PUT)(SetRegionOwnerHandler(m_game));
	CROW_ROUTE(app, "/initializeregion")
		.methods(crow::HTTPMethod::PUT)(InitializeRegionHandler(m_status, m_game));
	CROW_ROUTE(app, "/increaseregionscore")
		.methods(crow::HTTPMethod::PUT)(IncreaseRegionScoreHandler(m_game));
	CROW_ROUTE(app, "/decreaseregionscore")
		.methods(crow::HTTPMethod::PUT)(DecreaseRegionScoreHandler(m_game));
	//
	CROW_ROUTE(app, "/addresponse")
		.methods(crow::HTTPMethod::PUT)(AddResponseHandler(m_status, m_game, m_attackedRegion, m_leaderboard));
	CROW_ROUTE(app, "/getleaderboard")(GetLeaderboardHandler(m_status, m_game, m_leaderboard));
	CROW_ROUTE(app, "/setattackedterritory")
		.methods(crow::HTTPMethod::PUT)(SetAttackedTerritoryHandler(m_status, m_attacked, m_game, m_attackedRegion));
	//
}

void AppManager::GenereatePlayerOrder()
{
	//defining the possible number of rounds
	const int noOfRoundsFor2Player = 5;
	const int noOfRoundsFor3Player = 4;
	const int noOfRoundsFor4Player = 5;

	std::set<int> order;
	uint8_t numberOfPlayers = m_game.GetActivePlayers().size();
	std::vector<int> intermediatePhase;

	//getting the actual round number
	int currentNoOfRounds;
	if (numberOfPlayers == 2)
	{
		currentNoOfRounds = noOfRoundsFor2Player;
	}
	else if (numberOfPlayers == 3) {
		currentNoOfRounds = noOfRoundsFor3Player;
	}
	else if (numberOfPlayers == 4) {
		currentNoOfRounds = noOfRoundsFor4Player;
	}

	//initialising a vector with the numbers from interval 0 -> number of players-1
	for (uint8_t i = 0; i < numberOfPlayers; i++) {
		intermediatePhase.push_back(i);
	}

	//inserting in the queue
	while (m_playerOrder.size() != currentNoOfRounds * numberOfPlayers) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//shuffling the indexes
		shuffle(intermediatePhase.begin(), intermediatePhase.end(), std::default_random_engine(seed));

		//copying values in the queue
		for (auto& index : intermediatePhase) {
			m_playerOrder.push(m_waitingRoomList[index].GetUsername());
		}
	}
}