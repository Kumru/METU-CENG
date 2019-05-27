#include"Priest.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Priest::Priest(unsigned id, int x, int y, Team team)
	: Player(id, x, y, team)
{
	maxHP = 150;
	HP = 150;
	damage = 0;
	healPower = 50;
	goals.push_back(HEAL);
	goals.push_back(TO_ALLY);
	goals.push_back(CHEST);

	if (team == BARBARIANS)
		abbreviation = "PR";
	else
		abbreviation = "pr";
}

std::vector<Coordinate> Priest::getMoveableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(8);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });
	result.push_back({ coordinate.x + 1, coordinate.y + 1 });
	result.push_back({ coordinate.x + 1, coordinate.y - 1 });
	result.push_back({ coordinate.x - 1, coordinate.y - 1 });
	result.push_back({ coordinate.x - 1, coordinate.y + 1 });

	return result;
}

std::vector<Coordinate> Priest::getHealableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(8);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });
	result.push_back({ coordinate.x + 1, coordinate.y + 1 });
	result.push_back({ coordinate.x + 1, coordinate.y - 1 });
	result.push_back({ coordinate.x - 1, coordinate.y - 1 });
	result.push_back({ coordinate.x - 1, coordinate.y + 1 });

	return result;
}
