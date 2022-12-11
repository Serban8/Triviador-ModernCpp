#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TriviadorClient.h"
#include "cpr/cpr.h"
#include "crow.h"
#include <QLabel>
#include <thread>
#include <chrono>
class TriviadorClient : public QMainWindow
{
    Q_OBJECT

public:
    TriviadorClient(QWidget *parent = nullptr);
    ~TriviadorClient();
    int LoginFunction(std::string& username, std::string& password);
    int CreateNewPlayer(std::string& username, std::string& password);
    bool checkIfGameCanStart();
private:
    Ui::TriviadorclientClass ui;
};
