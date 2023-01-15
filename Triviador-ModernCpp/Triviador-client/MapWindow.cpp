#include "MapWindow.h"

MapWindow::MapWindow(QWidget* parent)
	: QMainWindow(parent)
{
	twoPlayersMap_ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);
	m_ShowQuestionsTimer = new QTimer(this);
	m_leaderboard = new QListWidget(this);
	connect(m_ShowQuestionsTimer, SIGNAL(timeout()), this, SLOT(StopTimer()));
}


MapWindow::~MapWindow()
{}

void MapWindow::SetUI(int numberOfPlayers)
{
	if (numberOfPlayers == 2)
	{
		twoPlayersMap_ui.setupUi(this);
		setTwoPlayersRegions();
		setTwoPlayersNeighbors();
	}
	else
		if (numberOfPlayers == 3)
		{
			threePlayersMap_ui.setupUi(this);
			setThreePlayersRegions();
			setThreePlayersNeighbors();
		}
		else
			if (numberOfPlayers == 4)
			{
				fourPlayersMap_ui.setupUi(this);
				setFourPlayersRegions();
				setFourPlayersNeighbors();
			}
	std::this_thread::sleep_for(std::chrono::seconds(15));
	ShowNumberQuestions();
}

void MapWindow::setTwoPlayersRegions()
{
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory1);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory2);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory3);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory4);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory5);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory6);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory7);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory8);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory9);
}

void MapWindow::setThreePlayersRegions()
{
	Regions.push_back(threePlayersMap_ui.groupBox_Territory1);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory2);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory3);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory4);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory5);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory6);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory7);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory8);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory9);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory10);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory11);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory12);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory13);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory14);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory15);
}

void MapWindow::setFourPlayersRegions()
{
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory1);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory2);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory3);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory4);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory5);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory6);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory7);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory8);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory9);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory10);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory11);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory12);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory13);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory14);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory15);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory16);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory17);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory18);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory19);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory20);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory21);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory22);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory23);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory24);
}

void MapWindow::setTwoPlayersNeighbors()
{
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory1] = { twoPlayersMap_ui.groupBox_Territory2,twoPlayersMap_ui.groupBox_Territory3,twoPlayersMap_ui.groupBox_Territory4,twoPlayersMap_ui.groupBox_Territory6,twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory8 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory2] = { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory3,twoPlayersMap_ui.groupBox_Territory6 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory3] = { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory2,twoPlayersMap_ui.groupBox_Territory4 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory4] = { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory3,twoPlayersMap_ui.groupBox_Territory8 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory5] = { twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory9 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory6] = { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory2,twoPlayersMap_ui.groupBox_Territory7 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory7] = { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory5,twoPlayersMap_ui.groupBox_Territory6,twoPlayersMap_ui.groupBox_Territory8,twoPlayersMap_ui.groupBox_Territory9 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory8] = { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory4,twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory9 };
	RegionNeighbors[twoPlayersMap_ui.groupBox_Territory9] = { twoPlayersMap_ui.groupBox_Territory5,twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory8 };
}

void MapWindow::setThreePlayersNeighbors()
{
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory1] = { threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory6 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory2] = { threePlayersMap_ui.groupBox_Territory1,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory6,threePlayersMap_ui.groupBox_Territory9 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory3] = { threePlayersMap_ui.groupBox_Territory1,threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory6,threePlayersMap_ui.groupBox_Territory12 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory4] = { threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory6,threePlayersMap_ui.groupBox_Territory7,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory13 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory5] = { threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory9,threePlayersMap_ui.groupBox_Territory10,threePlayersMap_ui.groupBox_Territory11,threePlayersMap_ui.groupBox_Territory12 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory6] = { threePlayersMap_ui.groupBox_Territory1,threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory4 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory7] = { threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory13 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory8] = { threePlayersMap_ui.groupBox_Territory11,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory13,threePlayersMap_ui.groupBox_Territory14,threePlayersMap_ui.groupBox_Territory15 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory9] = { threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory10 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory10] = { threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory9,threePlayersMap_ui.groupBox_Territory11 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory11] = { threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory10,threePlayersMap_ui.groupBox_Territory12 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory12] = { threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory7,threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory11,threePlayersMap_ui.groupBox_Territory13 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory13] = { threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory7,threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory14 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory14] = { threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory13,threePlayersMap_ui.groupBox_Territory15 };
	RegionNeighbors[threePlayersMap_ui.groupBox_Territory15] = { threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory14 };
}

void MapWindow::setFourPlayersNeighbors()
{
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory1] = { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory14 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory2] = { fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory6,fourPlayersMap_ui.groupBox_Territory14 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory3] = { fourPlayersMap_ui.groupBox_Territory1,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory20 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory4] = { fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory6,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory13,fourPlayersMap_ui.groupBox_Territory17 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory5] = { fourPlayersMap_ui.groupBox_Territory2,fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory6,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory14 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory6] = { fourPlayersMap_ui.groupBox_Territory2,fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory5 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory7] = { fourPlayersMap_ui.groupBox_Territory1,fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory8,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory14 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory8] = { fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory14 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory9] = { fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory17,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory22 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory10] = { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory8,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory14,fourPlayersMap_ui.groupBox_Territory19 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory11] = { fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory22 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory12] = { fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory22,fourPlayersMap_ui.groupBox_Territory24 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory13] = { fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory15,fourPlayersMap_ui.groupBox_Territory17 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory14] = { fourPlayersMap_ui.groupBox_Territory1,fourPlayersMap_ui.groupBox_Territory2,fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory8,fourPlayersMap_ui.groupBox_Territory10 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory15] = { fourPlayersMap_ui.groupBox_Territory13,fourPlayersMap_ui.groupBox_Territory17 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory16] = { fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory22 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory17] = { fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory13,fourPlayersMap_ui.groupBox_Territory15,fourPlayersMap_ui.groupBox_Territory22 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory18] = { fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory23,fourPlayersMap_ui.groupBox_Territory24 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory19] = { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory20,fourPlayersMap_ui.groupBox_Territory21 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory20] = { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory23 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory21] = { fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory20,fourPlayersMap_ui.groupBox_Territory22,fourPlayersMap_ui.groupBox_Territory23 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory22] = { fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory17,fourPlayersMap_ui.groupBox_Territory21 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory23] = { fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory20,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory24 };
	RegionNeighbors[fourPlayersMap_ui.groupBox_Territory24] = { fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory23 };
}

void MapWindow::ShowNumberQuestions() {

	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getnumberquestion" });
	auto numberQuestion = crow::json::load(response.text);
	std::string question = numberQuestion["question"].s();
	double correctAnswer = numberQuestion["correctAnswer"].d();
	double incorrectAnswer1 = numberQuestion["incorrectAnswer1"].d();
	double incorrectAnswer2 = numberQuestion["incorrectAnswer2"].d();
	double incorrectAnswer3 = numberQuestion["incorrectAnswer3"].d();

	QString qQuestion = QString::fromUtf8(question.c_str());
	QString qCorrectAnswer = QString::number(correctAnswer);
	QString qIncorrectAnswer1 = QString::number(incorrectAnswer1);
	QString qIncorrectAnswer2 = QString::number(incorrectAnswer2);
	QString qIncorrectAnswer3 = QString::number(incorrectAnswer3);

	question_widget = new QWidget(this);
	question_widget->resize(this->width() / 2, this->height() / 2);
	question_widget->move(this->width() / 2 - question_widget->geometry().width() / 2, this->height() / 2 - question_widget->geometry().height() / 2);
	question_widget->setStyleSheet("border-image: url(:/Triviadorclient/images/blankWhiteImage.jpg); border-radius: 10px;");

	question_label = new QLabel(question_widget);
	question_label->move(question_widget->width() / 2 - question_label->geometry().width() * 3, question_widget->height() / 5 - question_label->geometry().height() * 1.5);
	question_label->setText(qQuestion);
	question_label->resize(500, 100);
	//question_label->setScaledContents(true);
	question_label->setWordWrap(true);
	question_label->setStyleSheet("font-size : 25pt; border-image: white; border: none ; qproperty-alignment: AlignCenter;");

	answer_lineEdit = new QLineEdit(question_widget);
	answer_lineEdit->resize(300, 50);
	answer_lineEdit->setPlaceholderText("Write your answer here!");
	answer_lineEdit->setStyleSheet("font-size : 15pt; border-image: white;");
	answer_lineEdit->move(question_widget->width() / 2 - answer_lineEdit->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 4);
	//answer_lineEdit->hide();

	numericAnswer_pushButton = new QPushButton(question_widget);
	numericAnswer_pushButton->setText("Submit answer!");
	numericAnswer_pushButton->resize(question_widget->width() / 2, 30);
	numericAnswer_pushButton->move(question_widget->width() / 2 - numericAnswer_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() + numericAnswer_pushButton->geometry().height() /2);
	numericAnswer_pushButton->setStyleSheet("font-size : 10pt; border-image: white; border: 1px solid black; background-color: darkGray; ");

	m_ShowQuestionsTimer->start(30000);
}

void MapWindow::ShowMultipleChoiceQuestions()
{
	std::cout << m_playerUsername << std::endl;
	cpr::Response response = cpr::Put(cpr::Url{ "http://localhost:18080/getmultiplechoicequestion" },
		cpr::Payload{
			{"username", m_playerUsername}
		}
	);
	auto multipleChoiceQuestion = crow::json::load(response.text);
	std::string question = multipleChoiceQuestion["question"].s();
	std::string correctAnswer = multipleChoiceQuestion["correctAnswer"].s();
	std::string incorrectAnswer1 = multipleChoiceQuestion["incorrectAnswer1"].s();
	std::string incorrectAnswer2 = multipleChoiceQuestion["incorrectAnswer2"].s();
	std::string incorrectAnswer3 = multipleChoiceQuestion["incorrectAnswer3"].s();

	QString qQuestion = QString::fromUtf8(question.c_str());
	QString qCorrectAnswer = QString::fromUtf8(correctAnswer.c_str());
	QString qIncorrectAnswer1 = QString::fromUtf8(incorrectAnswer1.c_str());
	QString qIncorrectAnswer2 = QString::fromUtf8(incorrectAnswer2.c_str());
	QString qIncorrectAnswer3 = QString::fromUtf8(incorrectAnswer3.c_str());

	question_widget = new QWidget(this);
	question_widget->resize(this->width() / 2, this->height() / 2);
	question_widget->move(this->width() / 2 - question_widget->geometry().width() / 2, this->height() / 2 - question_widget->geometry().height() / 2);
	question_widget->setStyleSheet("border-image: url(:/Triviadorclient/images/blankWhiteImage.jpg); border-radius: 10px;");

	question_label = new QLabel(question_widget);
	question_label->move(question_widget->width() / 2 - question_label->geometry().width() * 3, question_widget->height() / 5 - question_label->geometry().height() * 1.5);
	question_label->setText(qQuestion);
	question_label->resize(500, 100);
	question_label->setStyleSheet("font-size : 25pt; border-image: white;");

	firstChoice_pushButton = new QPushButton(question_widget);
	firstChoice_pushButton->setText(qCorrectAnswer);
	firstChoice_pushButton->setStyleSheet("border-image: white;");
	//firstChoice_pushButton->move(question_widget->width() / 2  - firstChoice_pushButton->geometry().width() * 3, question_widget->height() / 2 + question_label->geometry().height() / 2);
	firstChoice_pushButton->resize(question_widget->width(), 30);
	firstChoice_pushButton->move(question_widget->width() / 2 - firstChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	//firstChoice_pushButton->hide();

	secondChoice_pushButton = new QPushButton(question_widget);
	secondChoice_pushButton->setText(qIncorrectAnswer1);
	secondChoice_pushButton->setStyleSheet("border-image: white;");
	//secondChoice_pushButton->move(question_widget->width() / 2 - secondChoice_pushButton->geometry().width() * 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	secondChoice_pushButton->resize(question_widget->width(), 30);
	secondChoice_pushButton->move(question_widget->width() / 2 - secondChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height());
	//secondChoice_pushButton->hide();

	thirdChoice_pushButton = new QPushButton(question_widget);
	thirdChoice_pushButton->setText(qIncorrectAnswer2);
	thirdChoice_pushButton->setStyleSheet("border-image: white;");
	//thirdChoice_pushButton->move(question_widget->width() / 2 + thirdChoice_pushButton->geometry().width(), question_widget->height() / 2 + question_label->geometry().height() / 2);
	thirdChoice_pushButton->resize(question_widget->width(), 30);
	thirdChoice_pushButton->move(question_widget->width() / 2 - thirdChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height() + thirdChoice_pushButton->geometry().height());
	//thirdChoice_pushButton->hide();

	fourthChoice_pushButton = new QPushButton(question_widget);
	fourthChoice_pushButton->setText(qIncorrectAnswer3);
	fourthChoice_pushButton->setStyleSheet("border-image: white;");
	fourthChoice_pushButton->move(question_widget->width() / 2 + fourthChoice_pushButton->geometry().width() * 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	fourthChoice_pushButton->resize(question_widget->width(), 30);
	fourthChoice_pushButton->move(question_widget->width() / 2 - fourthChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height() + thirdChoice_pushButton->geometry().height() + fourthChoice_pushButton->geometry().height());
	//fourthChoice_pushButton->hide();
	m_ShowQuestionsTimer->start(30000);

}

void MapWindow::UpdateMap()
{
	cpr::Response response = cpr::Get(cpr::Url());
}

void MapWindow::SetUsername(std::string playerUsername)
{
	m_playerUsername = playerUsername;
}

void MapWindow::HideWidget(QWidget*& widget)
{
	//m_ShowQuestionsTimer->stop();
	widget->hide();
}
void MapWindow::ShowLeaderboard()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getleaderboard" });
	crow::json::rvalue resBody;
	resBody = crow::json::load(response.text);
	for (int i = 1; i < resBody.size() ; i++)
	{
		std::string stringUsername = resBody["place"+std::to_string(i)].s();
		std::string stringPlace = "place" + std::to_string(i);
		QString username = QString::fromUtf8(stringUsername.c_str());
		QString place = QString::fromUtf8(stringPlace.c_str());
		m_leaderboard->addItem(place + "is" + username);

	}
}
void MapWindow::StopTimer()
{
	m_ShowQuestionsTimer->stop();
	HideWidget(question_widget);
}
