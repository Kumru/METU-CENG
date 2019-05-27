#include"Scout.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Scout::Scout(unsigned id, int x, int y, Team team)
	: Player(id, x, y, team)
{
	maxHP = 125;
	HP = 125;
	damage = 25;
	healPower = 0;
	goals.push_back(CHEST);
	goals.push_back(TO_ALLY);
	goals.push_back(ATTACK);

	if (team == BARBARIANS)
		abbreviation = "SC";
	else
		abbreviation = "sc";
}

std::vector<Coordinate> Scout::getAttackableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(8);

	result.push_back({ coordinate.x - 1, coordinate.y - 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x - 1, coordinate.y + 1 });
	result.push_back({ coordinate.x, coordinate.y - 1 });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x + 1, coordinate.y - 1 });
	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x + 1, coordinate.y + 1 });
	

	return result;
}

std::vector<Coordinate> Scout::getMoveableCoordinates()
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
