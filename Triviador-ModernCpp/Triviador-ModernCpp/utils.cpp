#include "utils.h"


std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> result;
	size_t startIndex = 0;

	for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex))
	{
		result.emplace_back(str.begin() + startIndex, str.begin() + found);
		startIndex = found + delim.size();
	}
	if (startIndex != str.size())
		result.emplace_back(str.begin() + startIndex, str.end());
	return result;
}

std::unordered_map<std::string, std::string> parseRequestBody(const std::string& urlArgs)
{
	if (urlArgs == "") {
		return {};
	}
	std::unordered_map<std::string, std::string> result;
	for (const auto& kvStr : split(urlArgs, "&")) {
		auto kvVector = split(kvStr, "=");
		if (kvVector.size() == 2)
			result[kvVector[0]] = kvVector[1];
	}
	return result;
}

int getRandNum(int min, int max) {
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(min, max);
	return distr(eng);
}

std::string ConvertStatusToString(const ServerStatus& status)
{
	/*
	WAIT_TO_START,
	START_ROUND,
	START_CHOOSING_REGIONS,
	CHOSE_BASE,
	CHOSE_REGION,
	DISPLAY_QUESTION,
	DISPLAY_LEADERBOARD,
	WAIT,
	CHOOSE_REGION,
	ATTACKER_CHOSE_REGION,
	GET_MULTIPLE_CHOICE_QUESTION,
	GET_NUMBER_QUESTION,
	UPDATE_MAP,
	END_ROUND,
	GAME_FINISHED,
	NONE
	*/
	switch (status)
	{
	case ServerStatus::WAIT_TO_START:
		return "WAIT_TO_START";
		break;
	case ServerStatus::START_ROUND:
		return "START_ROUND";
		break;
	case ServerStatus::START_CHOOSING_REGIONS:
		return "START_CHOOSING_REGIONS";
		break;
	case ServerStatus::CHOSE_BASE:
		return "CHOSE_BASE";
		break;
	case ServerStatus::CHOSE_REGION:
		return "CHOSE_REGION";
		break;
	case ServerStatus::DISPLAY_QUESTION:
		return "DISPLAY_QUESTION";
		break;
	case ServerStatus::DISPLAY_LEADERBOARD:
		return "DISPLAY_LEADERBOARD";
		break;
	case ServerStatus::WAIT:
		return "WAIT";
		break;
	case ServerStatus::CHOOSE_REGION:
		return "CHOOSE_REGION";
		break;
	case ServerStatus::ATTACKER_CHOSE_REGION:
		return "ATTACKER_CHOSE_REGION";
		break;
	case ServerStatus::GET_MULTIPLE_CHOICE_QUESTION:
		return "GET_MULTIPLE_CHOICE_QUESTION";
		break;
	case ServerStatus::GET_NUMBER_QUESTION:
		return "GET_NUMBER_QUESTION";
		break;
	case ServerStatus::UPDATE_MAP:
		return "UPDATE_MAP";
		break;
	case ServerStatus::END_ROUND:
		return "END_ROUND";
		break;
	case ServerStatus::GAME_FINISHED:
		return "GAME_FINISHED";
		break;
	default:
		throw std::runtime_error("Undefined status");
		return "Error";
		break;
	}
}
