#include "Map.h"

Map::Map(const uint8_t& numberOfPlayers)
{
	if (m_defaultDimensions.find(numberOfPlayers) != m_defaultDimensions.end()) {
		m_height = m_defaultDimensions[numberOfPlayers].first;
		m_width = m_defaultDimensions[numberOfPlayers].second;
		m_regions.resize(m_height * m_width);
	}
	else {
		throw std::runtime_error("Number of players must be between 2 and 4");
	}
}

Region& Map::operator[](const Position& pos)
{
	return m_regions[pos.first * m_width + pos.second];
}

const Region& Map::operator[](const Position& pos) const
{
	return m_regions[pos.first * m_width + pos.second];
}

int Map::GetHeight() const
{
	return m_height;
}

int Map::GetWidth() const
{
	return m_width;
}

bool Map::AreAllRegionsOwned() const
{
	for (const auto& region : m_regions) {
		if (!region.GetOwner().get()) {
			return false;
		}
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const Map& map)
{
	for (uint8_t i = 0; i < map.m_height; i++)
	{
		for (uint8_t j = 0; j < map.m_width; j++)
		{
			os << map[{i, j}].GetOwner().get()->GetUsername() << " | ";
		}
		os << std::endl;
		for (uint8_t j = 0; j < map.m_width; j++)
		{
			os << map[{i, j}].GetScore() << " | ";
		}
		os << std::endl;
		for (uint8_t j = 0; j < map.m_width; j++)
		{
			switch (map[{i, j}].GetType())
			{
			case Region::Type::Base:
				os << "Base" << " | ";
				break;
			case Region::Type::Territory:
				os << "Territory" << " | ";
				break;
			default:
				throw std::runtime_error("Undefined type of region");
				break;
			}
		}
		os << std::endl;

		os << std::endl << "--------------------------------------------------" << std::endl;
	}
	return os;
}
