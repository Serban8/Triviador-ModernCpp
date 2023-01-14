#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include "Player.h"

class Region
{
public:
	enum class Type : uint8_t {
		Base,
		Territory
	};

	//convert string to Type and vice-versa; maybe move in utils?
public:
	static inline Type StringToRegionType(std::string type) {
		if (type == "Base" || type == "base") {
			return Type::Base;
		}
		if (type == "Territory" || type == "territory") {
			return Type::Territory;
		}

		throw std::runtime_error("Undefined type of region");
	}

	static inline std::string RegionTypeToString(Type type) {
		switch (type)
		{
		case Region::Type::Base:
			return  "Base";
			break;
		case Region::Type::Territory:
			return "Territory";
			break;
		default:
			throw std::runtime_error("Undefined type of region");
			return "Error";
			break;
		}
	}

public:
	//constructor
	Region();
	Region(std::shared_ptr<Player> owner, Type type = Type::Territory, int score = 100);

	//setters
	void SetScore(int score);
	void SetOwner(std::shared_ptr<Player> owner);
	void SetType(Type type);

	//getters
	int GetScore() const;
	std::shared_ptr<Player> GetOwner() const;
	Type GetType() const;

	//methods
	void UpdateRegion(const std::shared_ptr<Player> winner);

private:
	static const int s_baseInitialScore = 300;
	static const int s_territoryInitialScore = 100;

private:
	int m_score;
	std::shared_ptr<Player> m_owner; //should be pointer
	Type m_type;
};