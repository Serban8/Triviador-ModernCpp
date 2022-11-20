#include "Map.h"

Map::Map(const uint8_t &numberOfPlayers)
{
	if (m_defaultDimensions.find(numberOfPlayers) != m_defaultDimensions.end()) {
		m_height = m_defaultDimensions[numberOfPlayers].first;
		m_width = m_defaultDimensions[numberOfPlayers].second;
		//for testing purposes
		std::cout << m_height << " " << m_width << std::endl; 
	}
	else {
		throw std::runtime_error("Number of players must be between 2 and 4");
	}
}

Region& Map::operator[](const Position& pos)
{
	return m_regions[pos.first * m_width + pos.second];
}
