#pragma once
#include <string>
#include <iostream>

//also needed in client
class Question {

public:
	Question() {}
	Question(std::string question, std::string category) :
		m_question(question),
		m_category(category)
	{
	}

	//getters
	virtual std::string GetQuestion() const;
	virtual std::string GetCategory() const;

	//operators
	friend  std::ostream& operator<<(std::ostream& out, const Question& question);

	//methods
	virtual void PrintQuestion() const;

protected:
	std::string m_question;
	std::string m_category;
};
