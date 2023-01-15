#pragma once

#include <QMainWindow>
#include <qscreen.h>
#include <qlineedit.h>
#include <qgraphicseffect.h>
#include <qtimer.h>
#include <QListWidget>

#include "ui_TwoPlayersMap.h"
#include "ui_ThreePlayersMap.h"
#include "ui_FourPlayersMap.h"

#include "crow.h"
#include "cpr/cpr.h"

#include "..\Triviador-DLL\ServerStatus.h"
#include "..\Triviador-DLL\MultipleChoiceQuestion.h"
#include "..\Triviador-DLL\NumberQuestion.h"

using AnyQuestion = std::variant<MultipleChoiceQuestion, NumberQuestion<int>, NumberQuestion<float>>;

class MapWindow : public QMainWindow
{
	Q_OBJECT

public:
	MapWindow(QWidget *parent = nullptr);
	~MapWindow();
	
	//initializers
	void SetUsername(std::string playerUsername);	
	
	void setTimer();
	
	void SetUI(int numberOfPlayers);
	void setTwoPlayersRegions();
	void setThreePlayersRegions();
	void setFourPlayersRegions();
	
	void setTwoPlayersNeighbors();
	void setThreePlayersNeighbors();
	void setFourPlayersNeighbors();
	void selectTwoPlayersRegion();
	void selectThreePlayersRegion();
	void selectFourPlayersRegion();
	//functions for different actions
	std::variant<NumberQuestion<int>, NumberQuestion<float>> GetNumberQuestion();
	MultipleChoiceQuestion GetMultipleChoiceQuestion();
	
	void ShowNumberQuestion();
	void ShowMultipleChoiceQuestion();

	void UpdateMap();
	void ShowLeaderboard();
	
	void HideWidget(QWidget*& widget);

private slots:
	void StopTimer();
	void on_numericAnswer_pushButton_clicked();
	void SetTwoPlayersRegion();
	void SetThreePlayersRegion();
	void SetFourPlayersRegion();

	void UpdateStatus();

private:
	QListWidget* m_leaderboard;
	std::string m_playerUsername;
	AnyQuestion m_currentQuestion;
	
	QTimer* m_showQuestionsTimer;
	QTimer* m_updateStatusTimer;
	
	//list of regions and neighbors
	std::vector<QGroupBox*> m_regions;
	std::unordered_map<QGroupBox*, std::vector<QGroupBox*>> m_regionNeighbors;

	//different uis according to the number of players
	Ui::TwoPlayersMapClass twoPlayersMap_ui;
	Ui::ThreePlayersMapClass threePlayersMap_ui;
	Ui::FourPlayersMapClass fourPlayersMap_ui;
	
	//different QT elements needed for the interface
	QLabel* question_label;
	QLineEdit* answer_lineEdit;
	QWidget* question_widget;
	QPushButton* firstChoice_pushButton;
	QPushButton* secondChoice_pushButton;
	QPushButton* thirdChoice_pushButton;
	QPushButton* fourthChoice_pushButton;
	QPushButton* numericAnswer_pushButton;
};
