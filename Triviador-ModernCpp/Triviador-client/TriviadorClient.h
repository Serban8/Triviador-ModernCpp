#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TriviadorClient.h"

class TriviadorClient : public QMainWindow
{
    Q_OBJECT

public:
    TriviadorClient(QWidget *parent = nullptr);
    ~TriviadorClient();

private:
    Ui::TriviadorclientClass ui;
};
