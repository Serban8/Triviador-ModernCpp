#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TriviadorClient.h"
#include "cpr/cpr.h"
#include "crow.h"
#include <QLabel>

class TriviadorClient : public QMainWindow
{
    Q_OBJECT

public:
    TriviadorClient(QWidget *parent = nullptr);
    ~TriviadorClient();
    int LoginFunction(std::string& username, std::string& password);
private:
    Ui::TriviadorclientClass ui;
};
