#include"Tank.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Tank::Tank(unsigned id, int x, int y, Team team)
	: Player(id, x, y, team)
{
	maxHP = 1000;
	HP = 1000;
	damage = 25;
	healPower = 0;
	goals.push_back(TO_ENEMY);
	goals.push_back(ATTACK);
	goals.push_back(CHEST);

	if (team == BARBARIANS)
		abbreviation = "TA";
	else
		abbreviation = "ta";
}

std::vector<Coordinate> Tank::getAttackableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(4);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });

	return result;
}

std::vector<Coordinate> Tank::getMoveableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(4);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });

	return result;
}
