#include "TriviadorClient.h"

TriviadorClient::TriviadorClient(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

TriviadorClient::~TriviadorClient()
{}

statusCode TriviadorClient::CheckLoginInfo(std::string& username, std::string& password)
{
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/checkplayer" },
		cpr::Payload{
			{ "username", username },
			{ "password", password }
		}
	);

	return response.status_code;
}

statusCode TriviadorClient::CreateNewPlayer(std::string& username, std::string& password)
{
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/addnewplayer" },
		cpr::Payload{
			{ "username", username },
			{ "password", password }
		}
	);

	return response.status_code;
}

void TriviadorClient::checkIfGameCanStart()
{
    crow::json::rvalue resBody; //the list of all players and their votes and the flag for starting the game
    do
    {
        cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
        resBody = crow::json::load(response.text);
        std::chrono::system_clock::time_point timePt = std::chrono::system_clock::now() + std::chrono::seconds(5);
        std::this_thread::sleep_until(timePt);

        //for each player display their option
        for (int i = 0; i < resBody.size() - 1; i++)
        {
            if (resBody[i]["votedToStart"] == "true")
            {
                std::cout << resBody[i]["username"] << " voted to start" << std::endl;
            }
            else
            {
                std::cout << resBody[i]["username"] << " did not vote" << std::endl;
            }
        }
    //if the flag is set to true then the game can start
    } while (resBody[resBody.size() - 1]["startGame"] != "true");
}
