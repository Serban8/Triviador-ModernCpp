#pragma once
#include <string>
#include <iostream>

#ifndef TRIVIADOR_EXPORTS
#define TRIVIADOR_API __declspec(dllexport)
#else
#define TRIVIADOR_API __declspec(dllimport)
#endif

class TRIVIADOR_API Question {

public:
	Question() = default;
	Question(std::string question, std::string category) :
		m_question(question),
		m_category(category)
	{
	}

	//getters
	virtual std::string GetQuestion() const;
	virtual std::string GetCategory() const;

	//operators
	friend TRIVIADOR_API std::ostream& operator<<(std::ostream& out, const Question& question);

	//methods
	virtual void PrintQuestion() const;

protected:
	std::string m_question;
	std::string m_category;
};
