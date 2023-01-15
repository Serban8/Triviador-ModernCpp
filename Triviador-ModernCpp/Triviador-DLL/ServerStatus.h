#pragma once
#ifndef TRIVIADOR_EXPORTS
#define TRIVIADOR_API __declspec(dllexport)
#else
#define TRIVIADOR_API __declspec(dllimport)
#endif

#include <cstdint>
#include <string>

enum class TRIVIADOR_API ServerStatus : uint8_t {
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
};

//Converting status to string
inline std::string TRIVIADOR_API ConvertStatusToString(const ServerStatus& status) {
	switch (status) {
	case ServerStatus::WAIT_TO_START:
		return "WAIT_TO_START";
	case ServerStatus::START_ROUND:
		return "START_ROUND";
	case ServerStatus::START_CHOOSING_REGIONS:
		return "START_CHOOSING_REGIONS";
	case ServerStatus::CHOSE_BASE:
		return "CHOSE_BASE";
	case ServerStatus::CHOSE_REGION:
		return "CHOSE_REGION";
	case ServerStatus::DISPLAY_QUESTION:
		return "DISPLAY_QUESTION";
	case ServerStatus::DISPLAY_LEADERBOARD:
		return "DISPLAY_LEADERBOARD";
	case ServerStatus::WAIT:
		return "WAIT";
	case ServerStatus::CHOOSE_REGION:
		return "CHOOSE_REGION";
	case ServerStatus::ATTACKER_CHOSE_REGION:
		return "ATTACKER_CHOSE_REGION";
	case ServerStatus::GET_MULTIPLE_CHOICE_QUESTION:
		return "GET_MULTIPLE_CHOICE_QUESTION";
	case ServerStatus::GET_NUMBER_QUESTION:
		return "GET_NUMBER_QUESTION";
	case ServerStatus::UPDATE_MAP:
		return "UPDATE_MAP";
	case ServerStatus::END_ROUND:
		return "END_ROUND";
	case ServerStatus::GAME_FINISHED:
		return "GAME_FINISHED";
	case ServerStatus::NONE:
		return "NONE";
	}
}

//Converting string to status
inline ServerStatus TRIVIADOR_API ConvertStringToStatus(const std::string& status) {
	if (status == "WAIT_TO_START") {
		return ServerStatus::WAIT_TO_START;
	}
	else if (status == "START_ROUND") {
		return ServerStatus::START_ROUND;
	}
	else if (status == "START_CHOOSING_REGIONS") {
		return ServerStatus::START_CHOOSING_REGIONS;
	}
	else if (status == "CHOSE_BASE") {
		return ServerStatus::CHOSE_BASE;
	}
	else if (status == "CHOSE_REGION") {
		return ServerStatus::CHOSE_REGION;
	}
	else if (status == "DISPLAY_QUESTION") {
		return ServerStatus::DISPLAY_QUESTION;
	}
	else if (status == "DISPLAY_LEADERBOARD") {
		return ServerStatus::DISPLAY_LEADERBOARD;
	}
	else if (status == "WAIT") {
		return ServerStatus::WAIT;
	}
	else if (status == "CHOOSE_REGION") {
		return ServerStatus::CHOOSE_REGION;
	}
	else if (status == "ATTACKER_CHOSE_REGION") {
		return ServerStatus::ATTACKER_CHOSE_REGION;
	}
	else if (status == "GET_MULTIPLE_CHOICE_QUESTION") {
		return ServerStatus::GET_MULTIPLE_CHOICE_QUESTION;
	}
	else if (status == "GET_NUMBER_QUESTION") {
		return ServerStatus::GET_NUMBER_QUESTION;
	}
	else if (status == "UPDATE_MAP") {
		return ServerStatus::UPDATE_MAP;
	}
	else if (status == "END_ROUND") {
		return ServerStatus::END_ROUND;
	}
	else if (status == "GAME_FINISHED") {
		return ServerStatus::GAME_FINISHED;
	}
	else if (status == "NONE") {
		return ServerStatus::NONE;
	}
}