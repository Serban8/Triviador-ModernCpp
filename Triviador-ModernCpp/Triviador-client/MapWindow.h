#pragma once

#include <QMainWindow>
#include <qscreen.h>
#include <qlineedit.h>
#include <qgraphicseffect.h>
#include "ui_TwoPlayersMap.h"
#include "ui_ThreePlayersMap.h"
#include "ui_FourPlayersMap.h"

class MapWindow : public QMainWindow
{
	Q_OBJECT

public:
	MapWindow(QWidget *parent = nullptr);
	~MapWindow();
	void SetUI(int numberOfPlayers);
	void setTwoPlayersRegions();
	void setThreePlayersRegions();
	void setFourPlayersRegions();
	void setTwoPlayersNeighbors();
	void setThreePlayersNeighbors();
	void setFourPlayersNeighbors();
	void ShowQuestions();

private:
	Ui::TwoPlayersMapClass twoPlayersMap_ui;
	Ui::ThreePlayersMapClass threePlayersMap_ui;
	Ui::FourPlayersMapClass fourPlayersMap_ui;
	std::vector<QGroupBox*> Regions;
	std::unordered_map<QGroupBox*, std::vector<QGroupBox*>> RegionNeighbors;
	QLabel* question_label;
	QLineEdit* answer_lineEdit;
	QWidget* question_widget;
	QPushButton* firstChoice_pushButton;
	QPushButton* secondChoice_pushButton;
	QPushButton* thirdChoice_pushButton;
	QPushButton* fourthChoice_pushButton;
	QTimer* m_ShowQuestionsTimer;
	QPushButton* numericAnswer_pushButton;

};
