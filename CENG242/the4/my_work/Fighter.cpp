#include"Fighter.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Fighter::Fighter(unsigned id, int x, int y, Team team)
	: Player(id, x, y, team)
{
	maxHP = 400;
	HP = 400;
	damage = 100;
	healPower = 0;
	goals.push_back(ATTACK);
	goals.push_back(TO_ENEMY);
	goals.push_back(CHEST);

	if (team == BARBARIANS)
		abbreviation = "FI";
	else
		abbreviation = "fi";
}

std::vector<Coordinate> Fighter::getAttackableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(4);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });

	return result;
}

std::vector<Coordinate> Fighter::getMoveableCoordinates()
{
	std::vector<Coordinate> result;
	result.reserve(4);

	result.push_back({ coordinate.x + 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y + 1 });
	result.push_back({ coordinate.x - 1, coordinate.y });
	result.push_back({ coordinate.x, coordinate.y - 1 });

	return result;
}