#include "TriviadorClient.h"

TriviadorClient::TriviadorClient(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //TESTING
    cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:18080/question" });
    QString qs = QString::fromStdString(r.text);
    //QLabel* label = new QLabel(this);
    //label->setText(qs);
    ui.label->setText(qs);
    //
}

TriviadorClient::~TriviadorClient()
{}

int TriviadorClient::LoginFunction(std::string & username, std::string & password)
{
		cpr::Response response;

		response = cpr::Post(
			cpr::Url{ " http://18080/checkplayer "/* not sure */ },
			cpr::Payload{
				{ " username ", username },
				{ " password ", password }
			}
		);

		return response.status_code;
}

int TriviadorClient::CreateNewPlayer(std::string& username, std::string& password)
{
		auto response = cpr::Post(
			cpr::Url{ " http://18080/addNewplayer "/* not sure */ },
			cpr::Payload{
				{ " username ", username },
				{ " password ", password }
			}
		);

		return response.status_code;
}

bool TriviadorClient::checkIfGameCanStart()
{
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
    auto players = crow::json::load(response.text);
    while (players.size() < 4)
    {
        std::chrono::system_clock::time_point timePt = std::chrono::system_clock::now() + std::chrono::seconds(5);
        std::this_thread::sleep_until(timePt);
        cpr::Response response2 = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
        auto players = crow::json::load(response2.text);
        if (players.size() == 4)
            break;
    }
	return true;
}
