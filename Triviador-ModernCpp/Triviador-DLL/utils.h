#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <random>
#include <variant>

#include "ServerStatus.h"
#include "NumberQuestion.h"

#ifndef TRIVIADOR_EXPORTS
#define TRIVIADOR_API __declspec(dllexport)
#else
#define TRIVIADOR_API __declspec(dllimport)
#endif

//Maybe use unnamed namespace (https://stackoverflow.com/questions/357404/why-are-unnamed-namespaces-used-and-what-are-their-benefits)
//Basically this means that if another symbol with the same name is defined elsewhere there will not be a violation of the One Definition Rule (ODR) -> does not thow compilation error.

std::vector<std::string> TRIVIADOR_API split(const std::string& str, const std::string& delim);

std::unordered_map<std::string, std::string> TRIVIADOR_API parseRequestBody(const std::string& urlArgs);

int TRIVIADOR_API getRandNum(int min, int max);