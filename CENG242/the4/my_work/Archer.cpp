#include"Archer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Archer::Archer(unsigned id, int x, int y, Team team)
	: Player(id, x, y, team)
{
	maxHP = 200;
	HP = 200;
	damage = 50;
	healPower = 0;

	goals.push_back(ATTACK);

	if (team == BARBARIANS)
		abbreviation = "AR";
	else
		abbreviation = "ar";
}

std::vector<Coordinate> Archer::getAttackableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(8);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x + 2, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x, coordinate.y + 2 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x - 2, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });
	result.push_back({ coordinate.x, coordinate.y - 2 });

	return result;
}