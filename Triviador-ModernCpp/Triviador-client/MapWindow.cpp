#include "MapWindow.h"
#include <qmessagebox.h>

MapWindow::MapWindow(QWidget* parent)
	: QMainWindow(parent)
{
	twoPlayersMap_ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);

	m_showQuestionsTimer = new QTimer(this);
	m_updateStatusTimer = new QTimer(this);

	m_InformationMsgBox.setStyleSheet("border-image: white;");
	m_InformationMsgBox.setIcon(QMessageBox::Information);
	m_InformationMsgBox.setWindowIcon(QIcon(pixmap));

	m_WarningMsgBox.setStyleSheet("border-image: white;");
	m_WarningMsgBox.setIcon(QMessageBox::Warning);
	m_WarningMsgBox.setWindowIcon(QIcon(pixmap));

	connect(m_showQuestionsTimer, SIGNAL(timeout()), this, SLOT(StopTimer()));
	connect(m_updateStatusTimer, SIGNAL(timeout()), this, SLOT(UpdateStatus()));
}

MapWindow::~MapWindow()
{}

//This is the first function called when entering this widget
void MapWindow::SetUI(int numberOfPlayers)
{

	if (numberOfPlayers == 2)
	{
		twoPlayersMap_ui.setupUi(this);
		setTwoPlayersRegions();
	}
	else if (numberOfPlayers == 3)
	{
		threePlayersMap_ui.setupUi(this);
		setThreePlayersRegions();
	}
	else if (numberOfPlayers == 4)
	{
		fourPlayersMap_ui.setupUi(this);
		setFourPlayersRegions();
	}
}

void MapWindow::UpdateMap()
{
	cpr::Response response = cpr::Get(cpr::Url());
}

void MapWindow::SetUsername(std::string playerUsername)
{
	m_playerUsername = playerUsername;
}

void MapWindow::setTimer()
{
	m_updateStatusTimer->start(2000);
	//UpdateStatus();
}

void MapWindow::HideWidget(QWidget*& widget)
{
	//m_showQuestionsTimer->stop();
	widget->hide();
}

void MapWindow::UpdateStatus()
{
	ServerStatus status;
	static bool questionIsMultipleChoice = false;
	static bool alreadyDisplayed = false;
	
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/checkstatus" },
		cpr::Payload{
			{ "username", m_playerUsername }
		});
	qDebug() << "RECEIVED STATUS: " << response.text.c_str();
	status = ConvertStringToStatus(crow::json::load(response.text)["status"].s());

	if (status == ServerStatus::GET_MULTIPLE_CHOICE_QUESTION) {
		questionIsMultipleChoice = true;
		m_currentQuestion = GetMultipleChoiceQuestion();
	}
	else if (status == ServerStatus::GET_NUMBER_QUESTION) {
		questionIsMultipleChoice = false;
		auto numberQuestion = GetNumberQuestion();
		if (std::holds_alternative<NumberQuestion<int>>(numberQuestion)) {
			m_currentQuestion = std::get<NumberQuestion<int>>(numberQuestion);
		}
		else {
			m_currentQuestion = std::get<NumberQuestion<float>>(numberQuestion);
		}
	}
	else if (status == ServerStatus::DISPLAY_QUESTION && !alreadyDisplayed) {
		alreadyDisplayed = true;
		if (questionIsMultipleChoice)
			ShowMultipleChoiceQuestion();
		else
			ShowNumberQuestion();

		//send response
		//int time = 2;
		//std::string res;

		//auto correctAnswer = question["correctAnswer"].d();
		//send over the distance to the correct response - lowest distance wins
		//auto dif = abs(stof(res) - correctAnswer);
		//auto response = cpr::Put(
		//	cpr::Url{ "http://localhost:18080/addresponse" },
		//	cpr::Payload{
		//		{ "username", m_playerUsername},
		//		{ "response", std::to_string(dif) },
		//		{ "time", std::to_string(time) }
		//	}
		//);

	}
	else if (status == ServerStatus::DISPLAY_LEADERBOARD) {
		question_widget->hide();
		alreadyDisplayed = false;
		//auto response = cpr::Get(cpr::Url{ "http://localhost:18080/getleaderboard" });
		//std::cout << std::endl << "---LEADERBOARD---" << std::endl;
		//std::cout << std::endl << response.text << std::endl;
		ShowLeaderboard();
	}
	else if (status == ServerStatus::CHOOSE_REGION) {
	//	std::cout << "Chose region 1,1 " << std::endl;
	//	cpr::Put(
	//		cpr::Url{ "http://localhost:18080/initializeregion" },
	//		cpr::Payload{
	//			{ "username", m_playerUsername},
	//			{ "line", "1" },
	//			{ "column", "1" },
	//			{ "type", "base"}
	//		}
	//	);
	}
	else if (status == ServerStatus::UPDATE_MAP) {
		//cpr::Get(cpr::Url{ "http://localhost:18080/getmap" });
	}
	else if (status == ServerStatus::GAME_FINISHED) {
		m_updateStatusTimer->stop();
	}
}

void MapWindow::ShowLeaderboard()
{
	m_leaderboardBase = new QWidget(this);
	m_leaderboardList = new QListWidget(m_leaderboardBase);
	m_correctAnswerShown = new QLabel(m_leaderboardBase);
	m_leaderboardBase->resize(this->width() / 2, this->height() / 2);
	m_leaderboardList->resize(m_leaderboardBase->width()/2, m_leaderboardBase->height()/3);
	m_leaderboardBase->move(this->width() / 2 - m_leaderboardBase->geometry().width() / 2, this->height() / 2 - m_leaderboardBase->geometry().height() / 3);
	m_leaderboardBase->setStyleSheet("border-image: url(:/Triviadorclient/images/blankWhiteImage.jpg); border-radius: 10px;");
	m_leaderboardList->move(m_leaderboardBase->width() / 2 - m_leaderboardList->geometry().width(), m_leaderboardBase->height() / 2 - m_leaderboardList->geometry().height()/2);
	m_leaderboardList->setStyleSheet("font-size: 15pt;");
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getleaderboard" });
	qDebug() << m_leaderboardBase->width() / 2 << m_leaderboardBase->height() / 3;
	qDebug() << this->width() / 2 - m_leaderboardBase->geometry().width() / 2 << this->height() / 2 - m_leaderboardBase->geometry().height() / 3;
	qDebug() << m_leaderboardBase->width() / 2 - m_leaderboardList->geometry().width() * 3 << m_leaderboardBase->height() / 5 - m_leaderboardList->geometry().height() * 1.5;
	auto resBody = crow::json::load(response.text);
	for (int i = 0; i < resBody.size(); i++)
	{
		std::string stringUsername = resBody[i]["place" + std::to_string(i+1)].s();
		std::string stringPlace = "place" + std::to_string(i+1);
		QString username = QString::fromUtf8(stringUsername.c_str());
		QString place = QString::fromUtf8(stringPlace.c_str());
		m_leaderboardList->addItem(username+ " is "+place);
	}
	m_leaderboardList->update();
	if (std::holds_alternative<MultipleChoiceQuestion>(m_currentQuestion))
	{
		m_correctAnswerShown->setText("Correct answer was:" + QString::fromUtf8(std::get<MultipleChoiceQuestion>(m_currentQuestion).GetCorrectAnswer().c_str()));
	}
	else if (std::holds_alternative<NumberQuestion<int>>(m_currentQuestion))
	{
		m_correctAnswerShown->setText("Correct answer was:" + QString::fromUtf8(std::to_string(std::get<NumberQuestion<int>>(m_currentQuestion).GetCorrectAnswer()).c_str()));
	}
	else if (std::holds_alternative<NumberQuestion<float>>(m_currentQuestion)) {
		m_correctAnswerShown->setText("Correct answer was:" + QString::fromUtf8(std::to_string(std::get<NumberQuestion<float>>(m_currentQuestion).GetCorrectAnswer()).c_str()));
	}
	m_correctAnswerShown->resize(m_leaderboardBase->width(), 30);
	m_correctAnswerShown->setStyleSheet("font-size : 15pt; border-image: white; border: 1px solid black; background-color: darkGray;qproperty-aligment: AlignCenter");
	m_correctAnswerShown->move(m_leaderboardBase->width()/2 - m_correctAnswerShown->geometry().width()/2,m_leaderboardBase->height()/2+m_leaderboardList->geometry().height()/2);
	m_leaderboardBase->show();
	m_leaderboardList->show();
}
void MapWindow::StopTimer()
{
	m_showQuestionsTimer->stop();
	HideWidget(question_widget);
}

void MapWindow::ShowNumberQuestion() {
	QString questionText;
	if (std::holds_alternative<NumberQuestion<int>>(m_currentQuestion)) {
		questionText = QString::fromUtf8(std::get<NumberQuestion<int>>(m_currentQuestion).GetQuestion().c_str());
	}
	else {
		questionText = std::get<NumberQuestion<float>>(m_currentQuestion).GetQuestion().c_str();
	}

	question_widget = new QWidget(this);
	question_widget->resize(this->width() / 2, this->height() / 2);
	question_widget->move(this->width() / 2 - question_widget->geometry().width() / 2, this->height() / 2 - question_widget->geometry().height() / 2);
	question_widget->setStyleSheet("border-image: url(:/Triviadorclient/images/blankWhiteImage.jpg); border-radius: 10px;");

	question_label = new QLabel(question_widget);
	question_label->move(question_widget->width() / 2 - question_label->geometry().width() * 3, question_widget->height() / 5 - question_label->geometry().height() * 1.5);
	question_label->setText(questionText);
	question_label->resize(500, 100);
	question_label->setScaledContents(true);
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
	numericAnswer_pushButton->move(question_widget->width() / 2 - numericAnswer_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() + numericAnswer_pushButton->geometry().height() / 2);
	numericAnswer_pushButton->setStyleSheet("font-size : 10pt; border-image: white; border: 1px solid black; background-color: darkGray; ");
	
	connect(numericAnswer_pushButton, SIGNAL(clicked()), this, SLOT(on_numericAnswer_pushButton_clicked()));
	//m_showQuestionsTimer->start(30000);

	question_widget->show();
	qDebug() << "Question displayed";
	//m_showQuestionsTimer->start(30000);
}

void MapWindow::ShowMultipleChoiceQuestion()
{
	
	const auto& crtQuestion = std::get<MultipleChoiceQuestion>(m_currentQuestion);
	QString questionText = crtQuestion.GetQuestion().c_str();

	QString correctAnswer = crtQuestion.GetCorrectAnswer().c_str();
	QString incorrectAnswer1 = crtQuestion.GetIncorrectAnswers()[0].c_str();
	QString incorrectAnswer2 = crtQuestion.GetIncorrectAnswers()[1].c_str();
	QString incorrectAnswer3 = crtQuestion.GetIncorrectAnswers()[2].c_str();

	question_widget = new QWidget(this);
	question_widget->resize(this->width() / 2, this->height() / 2);
	question_widget->move(this->width() / 2 - question_widget->geometry().width() / 2, this->height() / 2 - question_widget->geometry().height() / 2);
	question_widget->setStyleSheet("border-image: url(:/Triviadorclient/images/blankWhiteImage.jpg); border-radius: 10px;");

	question_label = new QLabel(question_widget);
	question_label->move(question_widget->width() / 2 - question_label->geometry().width() * 3, question_widget->height() / 5 - question_label->geometry().height() * 1.5);
	question_label->setText(questionText);
	question_label->resize(500, 100);
	question_label->setStyleSheet("font-size : 25pt; border-image: white; border: none ; qproperty-alignment: AlignCenter;");

	firstChoice_pushButton = new QPushButton(question_widget);
	firstChoice_pushButton->setEnabled(true);
	firstChoice_pushButton->setText(correctAnswer);
	firstChoice_pushButton->setStyleSheet("font-size : 10pt; border-image: white; border: 1px solid black; background-color: darkGray;");
	//firstChoice_pushButton->move(question_widget->width() / 2  - firstChoice_pushButton->geometry().width() * 3, question_widget->height() / 2 + question_label->geometry().height() / 2);
	firstChoice_pushButton->resize(question_widget->width(), 30);
	firstChoice_pushButton->move(question_widget->width() / 2 - firstChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	//firstChoice_pushButton->hide();

	secondChoice_pushButton = new QPushButton(question_widget);
	secondChoice_pushButton->setEnabled(true);
	secondChoice_pushButton->setText(incorrectAnswer1);
	secondChoice_pushButton->setStyleSheet("font-size : 10pt; border-image: white; border: 1px solid black; background-color: darkGray;");
	//secondChoice_pushButton->move(question_widget->width() / 2 - secondChoice_pushButton->geometry().width() * 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	secondChoice_pushButton->resize(question_widget->width(), 30);
	secondChoice_pushButton->move(question_widget->width() / 2 - secondChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height());
	//secondChoice_pushButton->hide();

	thirdChoice_pushButton = new QPushButton(question_widget);
	thirdChoice_pushButton->setEnabled(true);
	thirdChoice_pushButton->setText(incorrectAnswer2);
	thirdChoice_pushButton->setStyleSheet("font-size : 10pt; border-image: white; border: 1px solid black; background-color: darkGray;");
	//thirdChoice_pushButton->move(question_widget->width() / 2 + thirdChoice_pushButton->geometry().width(), question_widget->height() / 2 + question_label->geometry().height() / 2);
	thirdChoice_pushButton->resize(question_widget->width(), 30);
	thirdChoice_pushButton->move(question_widget->width() / 2 - thirdChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height() + thirdChoice_pushButton->geometry().height());
	//thirdChoice_pushButton->hide();

	fourthChoice_pushButton = new QPushButton(question_widget);
	fourthChoice_pushButton->setEnabled(true);
	fourthChoice_pushButton->setText(incorrectAnswer3);
	fourthChoice_pushButton->setStyleSheet("font-size : 10pt; border-image: white; border: 1px solid black; background-color: darkGray;");
	fourthChoice_pushButton->move(question_widget->width() / 2 + fourthChoice_pushButton->geometry().width() * 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	fourthChoice_pushButton->resize(question_widget->width(), 30);
	fourthChoice_pushButton->move(question_widget->width() / 2 - fourthChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height() + thirdChoice_pushButton->geometry().height() + fourthChoice_pushButton->geometry().height());
	//fourthChoice_pushButton->hide();

	m_multipleChoiceButtons = { firstChoice_pushButton, secondChoice_pushButton, thirdChoice_pushButton, fourthChoice_pushButton };

	question_widget->show();
	m_showQuestionsTimer->start(30000);
	SelectAnswerButton();
}

std::variant<NumberQuestion<int>, NumberQuestion<float>> MapWindow::GetNumberQuestion()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getnumberquestion" });
	auto question = crow::json::load(response.text);

	std::string questionText = question["question"].s();
	std::string category = question["category"].s();

	if (question["type"] == "int")
	{
		int correctAnswer = question["correctAnswer"].i();
		int incorrectAnswer1 = question["incorrectAnswer1"].i();
		int incorrectAnswer2 = question["incorrectAnswer2"].i();
		int incorrectAnswer3 = question["incorrectAnswer3"].i();
		qDebug() << "LOADED NUMBER QUESTION";
		return NumberQuestion<int>{ questionText, category, correctAnswer, { incorrectAnswer1, incorrectAnswer2, incorrectAnswer3 } };
	}
	else if (question["type"] == "float")
	{
		float correctAnswer = question["correctAnswer"].d();
		float incorrectAnswer1 = question["incorrectAnswer1"].d();
		float incorrectAnswer2 = question["incorrectAnswer2"].d();
		float incorrectAnswer3 = question["incorrectAnswer3"].d();
		qDebug() << "LOADED NUMBER QUESTION";
		return NumberQuestion<float>{ questionText, category, correctAnswer, { incorrectAnswer1, incorrectAnswer2, incorrectAnswer3 } };
	}
	else
	{
		throw std::runtime_error("Unknown number question type");
	}
}

MultipleChoiceQuestion MapWindow::GetMultipleChoiceQuestion()
{
	cpr::Response response = cpr::Put(cpr::Url{ "http://localhost:18080/getmultiplechoicequestion" },
		cpr::Payload{
			{"username", m_playerUsername}
		}
	);
	auto question = crow::json::load(response.text);
	std::string questionText = question["question"].s();
	std::string category = question["category"].s();
	std::string correctAnswer = question["correctAnswer"].s();
	std::string incorrectAnswer1 = question["incorrectAnswer1"].s();
	std::string incorrectAnswer2 = question["incorrectAnswer2"].s();
	std::string incorrectAnswer3 = question["incorrectAnswer3"].s();

	return MultipleChoiceQuestion{ questionText, category, correctAnswer, { incorrectAnswer1, incorrectAnswer2, incorrectAnswer3 } };
}

void MapWindow::setTwoPlayersRegions()
{
	m_regions =
	{ twoPlayersMap_ui.groupBox_Territory1, twoPlayersMap_ui.groupBox_Territory2, twoPlayersMap_ui.groupBox_Territory3,
		twoPlayersMap_ui.groupBox_Territory4, twoPlayersMap_ui.groupBox_Territory5, twoPlayersMap_ui.groupBox_Territory6,
		twoPlayersMap_ui.groupBox_Territory7, twoPlayersMap_ui.groupBox_Territory8, twoPlayersMap_ui.groupBox_Territory9 };

	setTwoPlayersNeighbors();
}

void MapWindow::setThreePlayersRegions()
{
	m_regions =
	{ threePlayersMap_ui.groupBox_Territory1, threePlayersMap_ui.groupBox_Territory2, threePlayersMap_ui.groupBox_Territory3,
		threePlayersMap_ui.groupBox_Territory4, threePlayersMap_ui.groupBox_Territory5, threePlayersMap_ui.groupBox_Territory6,
		threePlayersMap_ui.groupBox_Territory7, threePlayersMap_ui.groupBox_Territory8, threePlayersMap_ui.groupBox_Territory9,
		threePlayersMap_ui.groupBox_Territory10, threePlayersMap_ui.groupBox_Territory11, threePlayersMap_ui.groupBox_Territory12,
		threePlayersMap_ui.groupBox_Territory13, threePlayersMap_ui.groupBox_Territory14, threePlayersMap_ui.groupBox_Territory15 };

	setThreePlayersNeighbors();
}

void MapWindow::setFourPlayersRegions()
{
	m_regions =
	{ fourPlayersMap_ui.groupBox_Territory1, fourPlayersMap_ui.groupBox_Territory2, fourPlayersMap_ui.groupBox_Territory3,
		fourPlayersMap_ui.groupBox_Territory4, fourPlayersMap_ui.groupBox_Territory5, fourPlayersMap_ui.groupBox_Territory6,
		fourPlayersMap_ui.groupBox_Territory7, fourPlayersMap_ui.groupBox_Territory8, fourPlayersMap_ui.groupBox_Territory9 ,
		fourPlayersMap_ui.groupBox_Territory10, fourPlayersMap_ui.groupBox_Territory11, fourPlayersMap_ui.groupBox_Territory12,
		fourPlayersMap_ui.groupBox_Territory13, fourPlayersMap_ui.groupBox_Territory14, fourPlayersMap_ui.groupBox_Territory15,
		fourPlayersMap_ui.groupBox_Territory16, fourPlayersMap_ui.groupBox_Territory17, fourPlayersMap_ui.groupBox_Territory18,
		fourPlayersMap_ui.groupBox_Territory19, fourPlayersMap_ui.groupBox_Territory20, fourPlayersMap_ui.groupBox_Territory21,
		fourPlayersMap_ui.groupBox_Territory22, fourPlayersMap_ui.groupBox_Territory23, fourPlayersMap_ui.groupBox_Territory24 };

	setFourPlayersNeighbors();
}

void MapWindow::setTwoPlayersNeighbors()
{
	m_regionNeighbors =
	{
		{ { twoPlayersMap_ui.groupBox_Territory1 }, { twoPlayersMap_ui.groupBox_Territory2,twoPlayersMap_ui.groupBox_Territory3,twoPlayersMap_ui.groupBox_Territory4,twoPlayersMap_ui.groupBox_Territory6,twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory8 } },
		{ { twoPlayersMap_ui.groupBox_Territory2 }, { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory3,twoPlayersMap_ui.groupBox_Territory6 } },
		{ { twoPlayersMap_ui.groupBox_Territory3 }, { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory2,twoPlayersMap_ui.groupBox_Territory4 } },
		{ { twoPlayersMap_ui.groupBox_Territory4 }, { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory3,twoPlayersMap_ui.groupBox_Territory8 } },
		{ { twoPlayersMap_ui.groupBox_Territory5 }, { twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory9 } },
		{ { twoPlayersMap_ui.groupBox_Territory6 }, { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory2,twoPlayersMap_ui.groupBox_Territory7 } },
		{ { twoPlayersMap_ui.groupBox_Territory7 }, { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory5,twoPlayersMap_ui.groupBox_Territory6,twoPlayersMap_ui.groupBox_Territory8,twoPlayersMap_ui.groupBox_Territory9 } },
		{ { twoPlayersMap_ui.groupBox_Territory8 }, { twoPlayersMap_ui.groupBox_Territory1,twoPlayersMap_ui.groupBox_Territory4,twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory9 } },
		{ { twoPlayersMap_ui.groupBox_Territory9 }, { twoPlayersMap_ui.groupBox_Territory5,twoPlayersMap_ui.groupBox_Territory7,twoPlayersMap_ui.groupBox_Territory8  }}
	};
}

void MapWindow::setThreePlayersNeighbors()
{
	m_regionNeighbors =
	{
		{ { threePlayersMap_ui.groupBox_Territory1 }, { threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory6 } },
		{ { threePlayersMap_ui.groupBox_Territory2 }, { threePlayersMap_ui.groupBox_Territory1,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory6,threePlayersMap_ui.groupBox_Territory9 } },
		{ { threePlayersMap_ui.groupBox_Territory3 }, { threePlayersMap_ui.groupBox_Territory1,threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory6,threePlayersMap_ui.groupBox_Territory12 } },
		{ { threePlayersMap_ui.groupBox_Territory4 }, { threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory6,threePlayersMap_ui.groupBox_Territory7,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory13 } },
		{ { threePlayersMap_ui.groupBox_Territory5 }, { threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory9,threePlayersMap_ui.groupBox_Territory10,threePlayersMap_ui.groupBox_Territory11,threePlayersMap_ui.groupBox_Territory12 } },
		{ { threePlayersMap_ui.groupBox_Territory6 }, { threePlayersMap_ui.groupBox_Territory1,threePlayersMap_ui.groupBox_Territory2,threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory4 } },
		{ { threePlayersMap_ui.groupBox_Territory7 }, { threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory13 } },
		{ { threePlayersMap_ui.groupBox_Territory8 }, { threePlayersMap_ui.groupBox_Territory11,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory13,threePlayersMap_ui.groupBox_Territory14,threePlayersMap_ui.groupBox_Territory15 } },
		{ { threePlayersMap_ui.groupBox_Territory9 }, { threePlayersMap_ui.groupBox_Territory2, threePlayersMap_ui.groupBox_Territory5, threePlayersMap_ui.groupBox_Territory10 } },
		{ { threePlayersMap_ui.groupBox_Territory10 }, { threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory9,threePlayersMap_ui.groupBox_Territory11 } },
		{ { threePlayersMap_ui.groupBox_Territory11 }, { threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory10,threePlayersMap_ui.groupBox_Territory12 } },
		{ { threePlayersMap_ui.groupBox_Territory12 }, { threePlayersMap_ui.groupBox_Territory3,threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory5,threePlayersMap_ui.groupBox_Territory7,threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory11,threePlayersMap_ui.groupBox_Territory13 } },
		{ { threePlayersMap_ui.groupBox_Territory13 }, { threePlayersMap_ui.groupBox_Territory4,threePlayersMap_ui.groupBox_Territory7,threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory12,threePlayersMap_ui.groupBox_Territory14 } },
		{ { threePlayersMap_ui.groupBox_Territory14 }, { threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory13,threePlayersMap_ui.groupBox_Territory15 } },
		{ { threePlayersMap_ui.groupBox_Territory15 }, { threePlayersMap_ui.groupBox_Territory8,threePlayersMap_ui.groupBox_Territory14  } }
	};


}
void MapWindow::on_numericAnswer_pushButton_clicked()
{
	if (answer_lineEdit->text() == "")
	{
		m_WarningMsgBox.setText("Please enter a number!");
		m_WarningMsgBox.exec();

		//QMessageBox::warning(this, "", "Please enter a number");
	}
	else
	{
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getnumberquestion" });
		auto numberQuestion = crow::json::load(response.text);
		double correctAnswer = numberQuestion["correctAnswer"].d();
		auto res = answer_lineEdit->text().toDouble();
		auto dif = abs(res -correctAnswer);
		double time = 1.2;
		cpr::Put(
			cpr::Url{ "http://localhost:18080/addresponse" },
			cpr::Payload{
				{ "username", m_playerUsername},
				{ "response", std::to_string(dif) },
				{ "time", std::to_string(time) }
			}
		);

		m_InformationMsgBox.setText("Please wait for the rest to respond!");
		m_InformationMsgBox.exec();

		//QMessageBox::information(this, "", "Please wait for the rest to respond");
	}
}

void MapWindow::SetTwoPlayersRegion()
{
	QPushButton* b = qobject_cast<QPushButton*>(sender());
	if (b == twoPlayersMap_ui.pushButton_Territory1)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory2)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory3)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory4)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory5)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory6)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory7)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory8)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == twoPlayersMap_ui.pushButton_Territory9)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
}

void MapWindow::SetThreePlayersRegion()
{
	QPushButton* b = qobject_cast<QPushButton*>(sender());
	if (b == threePlayersMap_ui.pushButton_Territory1)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory2)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory3)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory4)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory5)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory6)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory7)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory8)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory9)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory10)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory11)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory12)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory13)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory14)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == threePlayersMap_ui.pushButton_Territory15)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
}

void MapWindow::SetFourPlayersRegion()
{
	QPushButton* b = qobject_cast<QPushButton*>(sender());
	if (b == fourPlayersMap_ui.pushButton_Territory1)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory2)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory3)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory4)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "0"},
				{ "column", "3"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory5)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory6)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory7)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory8)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "1"},
				{ "column", "3"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory9)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory10)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory11)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory12)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "2"},
				{ "column", "3"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory13)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory14)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory15)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}
	if (b == fourPlayersMap_ui.pushButton_Territory16)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "3"},
				{ "column", "3"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory17)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory18)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory19)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory20)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "4"},
				{ "column", "3"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory21)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "5"},
				{ "column", "0"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory22)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "5"},
				{ "column", "1"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory23)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "5"},
				{ "column", "2"},
				{ "owner", m_playerUsername }
			});
	}if (b == fourPlayersMap_ui.pushButton_Territory24)
	{
		cpr::Put(
			cpr::Url{ "http://localhost:18080/setregionowner" },
			cpr::Payload{
				{ "line", "5"},
				{ "column", "3"},
				{ "owner", m_playerUsername }
			});
	}
}

void MapWindow::setFourPlayersNeighbors()
{
	m_regionNeighbors =
	{
		{ { fourPlayersMap_ui.groupBox_Territory1 }, { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory14 } },
		{ { fourPlayersMap_ui.groupBox_Territory2 }, { fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory6,fourPlayersMap_ui.groupBox_Territory14 } },
		{ { fourPlayersMap_ui.groupBox_Territory3 }, { fourPlayersMap_ui.groupBox_Territory1,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory20 } },
		{ { fourPlayersMap_ui.groupBox_Territory4 }, { fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory6,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory13,fourPlayersMap_ui.groupBox_Territory17 } },
		{ { fourPlayersMap_ui.groupBox_Territory5 }, { fourPlayersMap_ui.groupBox_Territory2,fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory6,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory14 } },
		{ { fourPlayersMap_ui.groupBox_Territory6 }, { fourPlayersMap_ui.groupBox_Territory2,fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory5 } },
		{ { fourPlayersMap_ui.groupBox_Territory7 }, { fourPlayersMap_ui.groupBox_Territory1,fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory8,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory14 } },
		{ { fourPlayersMap_ui.groupBox_Territory8 }, { fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory14 } },
		{ { fourPlayersMap_ui.groupBox_Territory9 }, { fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory17,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory22 } },
		{ { fourPlayersMap_ui.groupBox_Territory10 }, { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory8,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory14,fourPlayersMap_ui.groupBox_Territory19 } },
		{ { fourPlayersMap_ui.groupBox_Territory11 }, { fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory22 } },
		{ { fourPlayersMap_ui.groupBox_Territory12 }, { fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory22,fourPlayersMap_ui.groupBox_Territory24 } },
		{ { fourPlayersMap_ui.groupBox_Territory13 }, { fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory15,fourPlayersMap_ui.groupBox_Territory17 } },
		{ { fourPlayersMap_ui.groupBox_Territory14 }, { fourPlayersMap_ui.groupBox_Territory1,fourPlayersMap_ui.groupBox_Territory2,fourPlayersMap_ui.groupBox_Territory5,fourPlayersMap_ui.groupBox_Territory7,fourPlayersMap_ui.groupBox_Territory8,fourPlayersMap_ui.groupBox_Territory10 } },
		{ { fourPlayersMap_ui.groupBox_Territory15 }, { fourPlayersMap_ui.groupBox_Territory13,fourPlayersMap_ui.groupBox_Territory17 } },
		{ { fourPlayersMap_ui.groupBox_Territory16 }, { fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory22 } },
		{ { fourPlayersMap_ui.groupBox_Territory17 }, { fourPlayersMap_ui.groupBox_Territory4,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory13,fourPlayersMap_ui.groupBox_Territory15,fourPlayersMap_ui.groupBox_Territory22 } },
		{ { fourPlayersMap_ui.groupBox_Territory18 }, { fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory23,fourPlayersMap_ui.groupBox_Territory24 } },
		{ { fourPlayersMap_ui.groupBox_Territory19 }, { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory10,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory20,fourPlayersMap_ui.groupBox_Territory21 } },
		{ { fourPlayersMap_ui.groupBox_Territory20 }, { fourPlayersMap_ui.groupBox_Territory3,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory23 } },
		{ { fourPlayersMap_ui.groupBox_Territory21 }, { fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory19,fourPlayersMap_ui.groupBox_Territory20,fourPlayersMap_ui.groupBox_Territory22,fourPlayersMap_ui.groupBox_Territory23 } },
		{ { fourPlayersMap_ui.groupBox_Territory22 }, { fourPlayersMap_ui.groupBox_Territory9,fourPlayersMap_ui.groupBox_Territory11,fourPlayersMap_ui.groupBox_Territory12,fourPlayersMap_ui.groupBox_Territory16,fourPlayersMap_ui.groupBox_Territory17,fourPlayersMap_ui.groupBox_Territory21 } },
		{ { fourPlayersMap_ui.groupBox_Territory23 }, { fourPlayersMap_ui.groupBox_Territory18,fourPlayersMap_ui.groupBox_Territory20,fourPlayersMap_ui.groupBox_Territory21,fourPlayersMap_ui.groupBox_Territory24 } },
		{ { fourPlayersMap_ui.groupBox_Territory24 }, { fourPlayersMap_ui.groupBox_Territory12, fourPlayersMap_ui.groupBox_Territory18, fourPlayersMap_ui.groupBox_Territory23 } }
	};
}

void MapWindow::selectTwoPlayersRegion()
{
	for (auto& groupBox : m_regions)
	{
		for (auto& obj : groupBox->children())
		{
			if (obj->objectName().contains("pushButton"))
			{
				connect(obj, SIGNAL(clicked()), this, SLOT(SetTwoPlayersRegion()));
			}
		}
	}
}

void MapWindow::selectThreePlayersRegion()
{
	for (auto& groupBox : m_regions)
	{
		for (auto& obj : groupBox->children())
		{
			if (obj->objectName().contains("pushButton"))
			{
				connect(obj, SIGNAL(clicked()), this, SLOT(SetThreePlayersRegion()));
			}
		}
	}
}

void MapWindow::selectFourPlayersRegion()
{
	for (auto& groupBox : m_regions)
	{
		for (auto& obj : groupBox->children())
		{
			if (obj->objectName().contains("pushButton"))
			{
				connect(obj, SIGNAL(clicked()), this, SLOT(SetFourPlayersRegion()));
			}
		}
	}
}

void MapWindow::SelectAnswerButton() {

	for (auto& button : m_multipleChoiceButtons) {

		connect(button, SIGNAL(clicked()), this, SLOT(SetAnswerButton()));
		
	}

}

void MapWindow::SetAnswerButton() {

	QPushButton* b = qobject_cast<QPushButton*>(sender());
	
	auto multipleChoiceQuestion = std::get<MultipleChoiceQuestion>(m_currentQuestion);
	auto correctAnswer = multipleChoiceQuestion.GetCorrectAnswer();
	auto res = b->text().toUtf8().constData();
	double time = 1.2;
	if (res == correctAnswer) {


		cpr::Put(
			cpr::Url{ "http://localhost:18080/addresponse" },
			cpr::Payload{
				{"username", m_playerUsername},
				{"response", "0"},
				{"time", std::to_string(time)}
			}
		);
	}
	else {
		cpr::Put(
			cpr::Url{ "http://localhost:18080/addresponse" },
			cpr::Payload{
				{"username", m_playerUsername},
				{"response", "1"},
				{"time", std::to_string(time)}
			}
		);
	}

	for (auto& button : m_multipleChoiceButtons) {
		button->setEnabled(false);
	}

}
