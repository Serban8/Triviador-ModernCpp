#include "TriviadorClient.h"
#include <QtWidgets/QApplication>
#include<thread>
#include<chrono>

void LoginTest() {

	statusCode response;
	std::string username, password;

	do {
		std::cout << " username + password " << std::endl;
		std::cin >> username;
		std::cin >> password;

		TriviadorClient client;

		response = client.CheckLoginInfo(username, password);

		if (response == 401) {
			std::cout << "Please enter the right passowrd. \n" << std::endl;
		}
		else if (response == 404) {
			std::cout << "User not found. " << std::endl;
		}
	} while (response != 200);

	//todo: add option to exit this loop
	std::cout << "Login successful! " << std::endl;
}

void AddNewPlayerTest() {

	std::string username, password;
	statusCode responseCode;

	do {
		std::cout << "Enter username and password\n";
		std::cin >> username;
		std::cin >> password;

		TriviadorClient client;

		responseCode = client.CreateNewPlayer(username, password);

		if (responseCode == 200) {
			std::cout << "Player successfully created!" << std::endl;
		}
		else if (responseCode == 409) {
			std::cout << "This username is already taken. Please choose another one." << std::endl;
		}
	} while (responseCode != 200);
}

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
	std::cout << response.text << std::endl;
}
void getMultipleChoiceQuestionTest()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getmultiplechoicequestion" });
	std::cout << response.text << std::endl;
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
	/*TriviadorClient w;
	w.show();*/

	//AddNewPlayerTest();
	//LoginTest();
	//while (true)
	//	checkWaitingRoomTest();

	int x = 1;

	while (x != 0) {

		std::cout << "1 - Add a new player. " << std::endl;
		std::cout << "2 - Login " << std::endl;
		std::cout << "3 - Check waiting room." << std::endl;
		std::cout << "4 - Check number of votes." << std::endl;
		std::cout << "5 - Get Number Question" << std::endl;
		std::cout << "6 - Get Multiple Choice Question" << std::endl;
		std::cin >> x;
		std::cout << std::endl;

		switch (x) {
		default:
		{
			std::cout << "Eroare!" << std::endl;
			break;
		}
		case 1: {
			AddNewPlayerTest();
			break;
		}
		case 2: {
			LoginTest();
			break;
		}
		case 3: {
			checkWaitingRoomTest();
			break;
		}
		case 4: {
			numberOfVotes();
			break;
		}
		case 5: {
			getNumberQuestionTest();
			break;
		}
		case 6: {
			getMultipleChoiceQuestionTest();
			break;
		}
		}
	}

	return a.exec();
}
