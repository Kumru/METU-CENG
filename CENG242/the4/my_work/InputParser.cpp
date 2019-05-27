#include"InputParser.h"
#include <string>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Game * InputParser::parseGame()
{
	int boardSize; std::cin >> boardSize;
	
	int x, y; std::cin >> x >> y;
	Coordinate chestPos(x, y);
	
	int maxTurn; std::cin >> maxTurn;

	Game* game = new Game(maxTurn, boardSize, {x, y});

	int playerCount; std::cin >> playerCount;

	int id;  std::string cls, teamStr;
	Team team;
	for (int i = 0; i < playerCount; i++) {
		std::cin >> id;
		std::cin >> cls >> teamStr;
		std::cin >> x >> y;
		team = (teamStr == "BARBARIAN") ? BARBARIANS : KNIGHTS;
		game->addPlayer(id, x, y, team, cls);
	}

	return game;
}
