#include "TriviadorClient.h"
#include <QtWidgets/QApplication>
#include<chrono>

void checkWaitingRoomTest() {
	std::string username, vote;
	std::cout << "username: ";
	std::cin >> username;
	std::cout << std::endl;
	std::cout << "Do you want to start the game?\n";
	//std::cin >> vote;

	time_t start = time(NULL);
	time_t timer = 10; // player has 10 seconds to decide

	while (time(NULL) < start + timer && vote.empty()) {
		std::thread t1([&]() {
			std::cin >> vote;
			});
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		t1.detach();
	}

	if (vote == "yes" || vote == "Yes")
	{
		auto response8 = cpr::Put(
			cpr::Url{ "http://localhost:18080/addvote" },
			cpr::Payload{
				{ "username", username }
			});
	}

}

void numberOfVotes() {

	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
	auto waiting = crow::json::load(response.text);

	int nrOfVotes = 0;

	for (int i = 0; i < waiting.size() - 1; i++) {
		if (waiting[i]["votedToStart"] == "true")
			nrOfVotes++;
	}

	std::cout << "There are " << nrOfVotes << " players who voted to start." << std::endl;
}

void getNumberQuestionTest()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getnumberquestion" });
	cpr::Response response1 = cpr::Get(cpr::Url{ "http://localhost:18080/getplayers" });
	auto players_json = crow::json::load(response1.text);
	auto numberQuestion = crow::json::load(response.text);

	std::cout << numberQuestion["question"] << std::endl;
	double time = 1.2;//hardcoded value for time to test it
	for (const auto& player : players_json)
	{
		time = time - 0.1;
		std::string res;

		std::cout << player["username"] << " you can respond:" << std::endl;
		std::cin >> res;
		std::cout << std::endl;

		auto correctAnswer = numberQuestion["correctAnswer"].d();
		//send over the distance to the correct response - lowest distance wins
		auto dif = abs(stof(res) - correctAnswer);
		auto response = cpr::Put(
			cpr::Url{ "http://localhost:18080/addresponse" },
			cpr::Payload{
				{ "username", (player["username"].s())},
				{ "response", std::to_string(dif) },
				{ "time", std::to_string(time) }
			}
		);
	}

}

void  getMultipleChoiceQuestionTest()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getmultiplechoicequestion" });
	cpr::Response response1 = cpr::Get(cpr::Url{ "http://localhost:18080/getplayers" });
	auto players_json = crow::json::load(response1.text);
	auto numberQuestion = crow::json::load(response.text);
	std::cin.get();
	std::cout << numberQuestion["question"] << std::endl;
	std::cout << "A)" << numberQuestion["correctAnswer"] << std::endl;
	std::cout << "B)" << numberQuestion["incorrectAnswer1"] << std::endl;
	std::cout << "C)" << numberQuestion["incorrectAnswer2"] << std::endl;
	std::cout << "D)" << numberQuestion["incorrectAnswer3"] << std::endl;
	double time = 1.2;//hardcoded value for time to test it
	for (const auto& player : players_json)
	{
		time = time - 0.1;
		std::string res;

		std::cout << player["username"] << " you can respond:" << std::endl;
		std::getline(std::cin, res);

		auto correctAnswer = numberQuestion["correctAnswer"].s();
		std::string dif = (res != correctAnswer || res == "A" ? "1" : "0");
		qInfo() << (res == correctAnswer ? "1" : "0");
		auto response = cpr::Put(
			cpr::Url{ "http://localhost:18080/addresponse" },
			cpr::Payload{
				{ "username", (player["username"].s())},
				{ "response", dif },
				{ "time", std::to_string(time) }
			}
		);
	}

}
void getMap()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getmap" });
	std::cout << response.text << std::endl;
	auto map_json = crow::json::load(response.text);
	for (const auto& region : map_json)
	{
		std::cout << "line: " << region["line"].d() << std::endl;
		std::cout << "column: " << region["column"].d() << std::endl;
		std::cout << "owner: " << region["owner"].s() << std::endl;
		std::cout << "score: " << region["score"].d() << std::endl;
		std::cout << "type: " << region["type"].s() << std::endl;
		std::cout << "-------------" << std::endl;

	}
}
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	TriviadorClient w;
	w.resize(1366,768);
	w.show();

	//AddNewPlayerTest();
	//LoginTest();
	//while (true)
	//	checkWaitingRoomTest();

	//int x = 1;

	//while (x != 0) {

	//	std::cout << "1 - Add a new player. " << std::endl;
	//	std::cout << "2 - Login " << std::endl;
	//	std::cout << "3 - Check waiting room." << std::endl;
	//	std::cout << "4 - Check number of votes." << std::endl;
	//	std::cout << "5 - Get Number Question" << std::endl;
	//	std::cout << "6 - Get Multiple Choice Question" << std::endl;
	//	std::cin >> x;
	//	std::cout << std::endl;

	//	switch (x) {
	//	default:
	//	{
	//		std::cout << "Eroare!" << std::endl;
	//		break;
	//	}
	//	case 1: {
	//		AddNewPlayerTest();
	//		break;
	//	}
	//	case 2: {
	//		LoginTest();
	//		break;
	//	}
	//	case 3: {
	//		checkWaitingRoomTest();
	//		break;
	//	}
	//	case 4: {
	//		numberOfVotes();
	//		break;
	//	}
	//	case 5: {
	//		getNumberQuestionTest();
	//		break;
	//	}
	//	case 6: {
	//		getMultipleChoiceQuestionTest();
	//		break;
	//	}
	//	}
	//}

	return a.exec();
}
