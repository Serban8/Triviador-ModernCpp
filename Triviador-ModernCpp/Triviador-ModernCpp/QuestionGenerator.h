#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cpr/cpr.h>
#include <variant>

#include "..\Triviador-DLL\MultipleChoiceQuestion.h"
#include "..\Triviador-DLL\NumberQuestion.h"
#include "json.hpp"

using json = nlohmann::json;

//since the api can only retrieve 50 questions at a time, it provides a functionality that it generates a token and, 
//if you use that token in the query, it will not give you any repeating questions

/*
Response codes from opentdb.com:
Code 0: Success Returned results successfully.
Code 1: No Results Could not return results. The API doesn't have enough questions for your query. (Ex. Asking for 50 Questions in a Category that only has 20.)
Code 2: Invalid Parameter Contains an invalid parameter. Arguements passed in aren't valid. (Ex. Amount = Five)
Code 3: Token Not Found Session Token does not exist.
Code 4: Token Empty Session Token has returned all possible questions for the specified query. Resetting the Token is necessary.
*/

class QuestionGenerator
{
public:
	QuestionGenerator();
	std::vector<MultipleChoiceQuestion> GenerateMultipleChoiceQuestions(int numberOfQuestions = 100);
	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> GenerateNumberAnswerQuestions(int numberOfQuestions = 100);
	std::variant<NumberQuestion<int>, NumberQuestion<float>> GenerateQuestion(std::string question, std::string category, std::string correctAns, std::array<std::string, 3> incorrectAnswers, bool& isValid);

private:
	//multiple choice questions helpers
	std::string GenerateToken();
	std::vector<MultipleChoiceQuestion> GetMultipleChoiceQuestions(uint8_t numOfQuestions = 50);

private:
	//number answer questions helpers
	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> GetNumberAnswerQuestions(uint8_t numOfQuestions = 30);
	NumberQuestion<int> GenerateIntQuestion(std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers, bool& isValid);
	NumberQuestion<float> GenerateFloatQuestion(std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers, bool& isValid);

private:
	const std::string m_baseMultipleChoiceQuestionDatabaseUrl = "https://opentdb.com/";
	const std::string m_baseNumberAnswerQuestionDatabaseUrl = "https://the-trivia-api.com/";
	std::string m_token;
};
