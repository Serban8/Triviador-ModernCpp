#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <ctime>
#include <iostream>
#include <random>
#include <unordered_set>

#include "Player.h"
#include "..\Triviador-DLL\MultipleChoiceQuestion.h"
#include "..\Triviador-DLL\NumberQuestion.h"

namespace sql = sqlite_orm;

struct PlayerDatabase
{
	PlayerDatabase() = default;

	PlayerDatabase(Player p) :
		m_username(p.GetUsername())
	{
		//empty
	}

	std::string m_username;
	std::string m_password;

};

struct GameDatabase
{
	GameDatabase() = default;

	//winner is a FK to players table
	GameDatabase(std::string winner, int rounds) :
		m_winner(std::make_unique<std::string>(winner)),
		m_rounds(rounds)
	{
		//initialize time
		time_t now = time(0);
		tm ltm;
		localtime_s(&ltm, &now);
		//add year
		m_date.append(std::to_string(1900 + ltm.tm_year));
		m_date.append("-");
		//add month
		m_date.append(std::to_string(1 + ltm.tm_mon));
		m_date.append("-");
		//add day
		m_date.append(std::to_string(ltm.tm_mday));
	}

	int m_id;
	int m_rounds;
	std::unique_ptr<std::string> m_winner;
	std::string m_date;

};

//TO VERIFY: for tables made for N:M relations if we use Database class or game classes
struct PlayerGameDatabase {

	PlayerGameDatabase() = default;
	PlayerGameDatabase(int game, std::string player) :
		m_gameId(std::make_unique<int>(game)),
		m_playerId(std::make_unique<std::string>(player))
	{
		//empty
	}

	std::unique_ptr<int> m_gameId;
	std::unique_ptr<std::string> m_playerId;
};

struct QuestionDatabase
{

	QuestionDatabase() = default;
	QuestionDatabase(MultipleChoiceQuestion q) :
		m_question(q.GetQuestion()),
		m_category(q.GetCategory()),
		m_type("MultipleChoice"),
		m_correctAnswer(q.GetCorrectAnswer()),
		m_incorrectAnswer1(q.GetIncorrectAnswers()[0]),
		m_incorrectAnswer2(q.GetIncorrectAnswers()[1]),
		m_incorrectAnswer3(q.GetIncorrectAnswers()[2])
	{
	}
	QuestionDatabase(std::variant<NumberQuestion<int>, NumberQuestion<float>> nq) :
		m_type("Number")
	{
		if (std::holds_alternative<NumberQuestion<int>>(nq))
		{
			m_question = std::get<NumberQuestion<int>>(nq).GetQuestion();
			m_category = std::get<NumberQuestion<int>>(nq).GetCategory();
			m_correctAnswer = std::to_string(std::get<NumberQuestion<int>>(nq).GetCorrectAnswer());
			std::array<int, 3> auxArray = std::get<NumberQuestion<int>>(nq).GetIncorrectAnswers();
			m_incorrectAnswer1 = std::to_string(auxArray[0]);
			m_incorrectAnswer3 = std::to_string(auxArray[1]);
			m_incorrectAnswer2 = std::to_string(auxArray[2]);

		}
		else {
			m_question = std::get<NumberQuestion<float>>(nq).GetQuestion();
			m_category = std::get<NumberQuestion<float>>(nq).GetCategory();
			m_correctAnswer = std::to_string(std::get<NumberQuestion<float>>(nq).GetCorrectAnswer());
			std::array<float, 3> auxArray = std::get<NumberQuestion<float>>(nq).GetIncorrectAnswers();
			m_incorrectAnswer1 = std::to_string(auxArray[0]);
			m_incorrectAnswer3 = std::to_string(auxArray[1]);
			m_incorrectAnswer2 = std::to_string(auxArray[2]);
		}
	}

	int m_id;
	std::string m_question;
	std::string m_category;
	std::string m_type;
	std::string m_correctAnswer;
	std::string m_incorrectAnswer1;
	std::string m_incorrectAnswer2;
	std::string m_incorrectAnswer3;
};

inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table("Players",
			sql::make_column("username", &PlayerDatabase::m_username, sql::primary_key()),//or player, need to see
			sql::make_column("password", &PlayerDatabase::m_password)
			//instead of an column we will make a query to get no. of  games played
		),
		sql::make_table("Questions",
			sql::make_column("id", &QuestionDatabase::m_id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("question", &QuestionDatabase::m_question),
			sql::make_column("category", &QuestionDatabase::m_category),
			sql::make_column("type", &QuestionDatabase::m_type),
			sql::make_column("correctAnswer", &QuestionDatabase::m_correctAnswer),
			sql::make_column("incorrectAnswer1", &QuestionDatabase::m_incorrectAnswer1),
			sql::make_column("incorrectAnswer2", &QuestionDatabase::m_incorrectAnswer2),
			sql::make_column("incorrectAnswer3", &QuestionDatabase::m_incorrectAnswer3)
		),
		sql::make_table("Game",
			sql::make_column("id", &GameDatabase::m_id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("winner", &GameDatabase::m_winner),
			sql::make_column("rounds", &GameDatabase::m_rounds),
			sql::make_column("date", &GameDatabase::m_date),
			sql::foreign_key(&GameDatabase::m_winner).references(&PlayerDatabase::m_username)
		),
		sql::make_table("PlayerGames",
			sql::make_column("game", &PlayerGameDatabase::m_gameId),
			sql::make_column("username", &PlayerGameDatabase::m_playerId),
			sql::primary_key(&PlayerGameDatabase::m_gameId, &PlayerGameDatabase::m_playerId),
			sql::foreign_key(&PlayerGameDatabase::m_gameId).references(&GameDatabase::m_id),
			sql::foreign_key(&PlayerGameDatabase::m_playerId).references(&PlayerDatabase::m_username)
		)
	);
}

using Storage = decltype(createStorage(""));