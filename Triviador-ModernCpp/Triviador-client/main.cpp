#include "TriviadorClient.h"
#include <QtWidgets/QApplication>

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
	std::cin >> username;
	std::cout << "Do you want to start the game?\n";
	std::cin >> vote;
	if (vote == "yes" || vote == "Yes")
	{
		std::cout << "da" << std::endl;
		auto response8 = cpr::Put(
			cpr::Url{ "http://localhost:18080/addvote" },
			cpr::Payload{
				{ "username", username }
			});
	}
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	/*TriviadorClient w;
	w.show();*/

	//AddNewPlayerTest();
	//LoginTest();
	while(true)
		checkWaitingRoomTest();

	return a.exec();
}
