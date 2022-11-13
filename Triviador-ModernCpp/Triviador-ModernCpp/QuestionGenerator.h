#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cpr/cpr.h>

#include "MultipleChoiceQuestion.h"
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
	std::vector<MultipleChoiceQuestion> GenerateQuestions(int numberOfQuestions = 100);

private:
	const std::string m_baseOnlineDatabaseUrl = "https://opentdb.com/";
	std::string m_token;
	std::string GenerateToken();
	std::vector<MultipleChoiceQuestion> GetQuestionsFromOnlineDatabase(uint8_t numOfQuestions = 50);

};
