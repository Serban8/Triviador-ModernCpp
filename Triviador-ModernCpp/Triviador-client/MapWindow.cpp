#include "MapWindow.h"

MapWindow::MapWindow(QWidget* parent)
	: QMainWindow(parent)
{
	twoPlayersMap_ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);
}


MapWindow::~MapWindow()
{}

void MapWindow::SetUI(int numberOfPlayers)
{
	if (numberOfPlayers == 2)
	{
		twoPlayersMap_ui.setupUi(this);
		setTwoPlayersRegions();
	}
	else
		if (numberOfPlayers == 3)
		{
			threePlayersMap_ui.setupUi(this);
			setTwoPlayersRegions();
		}
		else
			if (numberOfPlayers == 4)
			{
				fourPlayersMap_ui.setupUi(this);
				ShowQuestions();
				setFourPlayersRegions();
			}
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

void MapWindow::ShowQuestions() {

	question_widget = new QWidget(this);
	question_widget->resize(this->width() / 2, this->height() / 2);
	question_widget->move(this->width() / 2 - question_widget->geometry().width() / 2, this->height() / 2 - question_widget->geometry().height() / 2);
	question_widget->setStyleSheet("border-image: url(:/Triviadorclient/images/blankWhiteImage.jpg);");

	question_label = new QLabel(question_widget);
	question_label->move(question_widget->width() / 2 - question_label->geometry().width() * 3, question_widget->height() / 5 - question_label->geometry().height() * 1.5);
	question_label->setText("My extraordinary question");
	question_label->resize(500, 100);
	question_label->setStyleSheet("font-size : 25pt; border-image: white;");

	firstChoice_pushButton = new QPushButton(question_widget);
	firstChoice_pushButton->setText("firstChoice");
	firstChoice_pushButton->setStyleSheet("border-image: white;");
	//firstChoice_pushButton->move(question_widget->width() / 2  - firstChoice_pushButton->geometry().width() * 3, question_widget->height() / 2 + question_label->geometry().height() / 2);
	firstChoice_pushButton->resize(question_widget->width(), 30);
	firstChoice_pushButton->move(question_widget->width() / 2 - firstChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	firstChoice_pushButton->hide();

	secondChoice_pushButton = new QPushButton(question_widget);
	secondChoice_pushButton->setText("secondChoice");
	secondChoice_pushButton->setStyleSheet("border-image: white;");
	//secondChoice_pushButton->move(question_widget->width() / 2 - secondChoice_pushButton->geometry().width() * 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	secondChoice_pushButton->resize(question_widget->width(), 30);
	secondChoice_pushButton->move(question_widget->width() / 2 - secondChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height());
	secondChoice_pushButton->hide();

	thirdChoice_pushButton = new QPushButton(question_widget);
	thirdChoice_pushButton->setText("thirdChoice");
	thirdChoice_pushButton->setStyleSheet("border-image: white;");
	//thirdChoice_pushButton->move(question_widget->width() / 2 + thirdChoice_pushButton->geometry().width(), question_widget->height() / 2 + question_label->geometry().height() / 2);
	thirdChoice_pushButton->resize(question_widget->width(), 30);
	thirdChoice_pushButton->move(question_widget->width() / 2 - thirdChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height() + thirdChoice_pushButton->geometry().height());
	thirdChoice_pushButton->hide();

	fourthChoice_pushButton = new QPushButton(question_widget);
	fourthChoice_pushButton->setText("fourthChoice");
	fourthChoice_pushButton->setStyleSheet("border-image: white;");
	fourthChoice_pushButton->move(question_widget->width() / 2 + fourthChoice_pushButton->geometry().width() * 2, question_widget->height() / 2 + question_label->geometry().height() / 2);
	fourthChoice_pushButton->resize(question_widget->width(), 30);
	fourthChoice_pushButton->move(question_widget->width() / 2 - fourthChoice_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 2 + secondChoice_pushButton->geometry().height() + thirdChoice_pushButton->geometry().height() + fourthChoice_pushButton->geometry().height());
	fourthChoice_pushButton->hide();

	answer_lineEdit = new QLineEdit(question_widget);
	answer_lineEdit->resize(300, 50);
	answer_lineEdit->setPlaceholderText("Here you can write your answer!");
	answer_lineEdit->setStyleSheet("font-size : 15pt; border-image: white;");
	answer_lineEdit->move(question_widget->width() / 2 - answer_lineEdit->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() / 4);
	//answer_lineEdit->hide();

	numericAnswer_pushButton = new QPushButton(question_widget);
	numericAnswer_pushButton->setText("Submit answer!");
	numericAnswer_pushButton->setStyleSheet("border-image: white;");
	numericAnswer_pushButton->resize(question_widget->width() / 2, 30);
	numericAnswer_pushButton->move(question_widget->width() / 2 - numericAnswer_pushButton->geometry().width() / 2, question_widget->height() / 2 + question_label->geometry().height() + numericAnswer_pushButton->geometry().height() /2);

}