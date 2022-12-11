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
