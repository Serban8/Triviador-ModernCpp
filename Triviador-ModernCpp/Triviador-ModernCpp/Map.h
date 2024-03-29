#pragma once
#include "Region.h"
#include <unordered_map>
#include <iostream>

class Map
{
public:
	using Position = std::pair<uint8_t, uint8_t>;

public:
	Map() = default;
	Map(const uint8_t& numberOfPlayers);

	//operators
	Region& operator[](const Position& pos);
	const Region& operator[](const Position& pos) const;
	friend std::ostream& operator<<(std::ostream& os, const Map& map);

	//getters
	int GetHeight() const;
	int GetWidth() const;

	//methods
	bool AreAllRegionsOwned() const;

private:
	int m_height;
	int m_width;
	std::vector<Region> m_regions;

private:
	std::unordered_map<uint8_t, Position> m_defaultDimensions =
	{
		{2, {3, 3}},
		{3, {5, 3}},
		{4, {6, 4}},
	};

};
