#include "TriviadorClient.h"
#include <QtWidgets/QApplication>

void TestingLogin() {

	int response = 0;
	std::string username, password;

	do {
		std::cout << " username + password " << std::endl;
		std::cin >> username;
		std::cin >> password;

		TriviadorClient player;

		response = player.LoginFunction(username, password);

		if (response == 401) {
			std::cout << "Password does not match username. \n" << std::endl;
			std::cout << "Enter right passowrd. \n" << std::endl;
		}
		else if (response == 404) {
			std::cout << "User not found. " << std::endl;
		}
	} while (response != 200);

	std::cout << "Login successful! " << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*TriviadorClient w;
    w.show();*/

	TestingLogin();

    return a.exec();
}
