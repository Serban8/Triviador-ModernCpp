#include "TriviadorClient.h"
#include <QtWidgets/QApplication>

void LoginTest() {

	int response;
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
	std::cin >> username;
	std::cin >> password;

	TriviadorClient player;

	statusCode response = player.CreateNewPlayer(username, password);

	if (response == 200) {
		std::cout << "Action successful!" << std::endl;
	}
	else {
		std::cout << "This username is already taken. Please choose another one." << std::endl;
	}
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	/*TriviadorClient w;
	w.show();*/

	AddNewPlayerTest();
	//LoginTest();

	return a.exec();
}
