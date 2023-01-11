#include "AppManager.h"

AppManager::AppManager(crow::SimpleApp& app, Storage& storage): m_app(app), m_storage(storage)
{
	//login related routes
	CROW_ROUTE(app, "/addnewplayer")
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::addNewPlayer(storage, req);
			});
	CROW_ROUTE(app, "/checkplayer")
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
	CROW_ROUTE(app, "/startgame")(StartGameHandler(m_game, m_waitingRoomList, storage));
	CROW_ROUTE(app, "/getplayers")(GetPlayersHandler(m_game));
	CROW_ROUTE(app, "/getnumberquestion")(GetNumberQuestionHandler(m_game));
	CROW_ROUTE(app, "/getmultiplechoicequestion")(GetMultipleChoiceQuestionHandler(m_game));
	//map related routes
	CROW_ROUTE(app, "/getmap")(GetMapHandler(m_game));
	CROW_ROUTE(app, "/setregionowner")
		.methods(crow::HTTPMethod::PUT)(SetRegionOwnerHandler(m_game));
	CROW_ROUTE(app, "/setregiontype")
		.methods(crow::HTTPMethod::PUT)(SetRegionTypeHandler(m_game));
	CROW_ROUTE(app, "/increaseregionscore")
		.methods(crow::HTTPMethod::PUT)(IncreaseRegionScoreHandler(m_game));
	CROW_ROUTE(app, "/decreaseregionscore")
		.methods(crow::HTTPMethod::PUT)(DecreaseRegionScoreHandler(m_game));
	//
	CROW_ROUTE(app, "/addresponse")
		.methods(crow::HTTPMethod::PUT)(AddResponseHandler(m_game, m_leaderboard));
	CROW_ROUTE(app, "/getleaderboard")(GetLeaderboardHandler(m_leaderboard));
	CROW_ROUTE(app, "/setattackedterritory")
		.methods(crow::HTTPMethod::PUT)(SetAttackedTerritoryHandler(m_game, m_attackedRegion));
	//
}
