#include"Game.h"

#include"Archer.h"
#include"Tank.h"
#include"Priest.h"
#include"Fighter.h"
#include"Scout.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

#define INT_MAX 2147483647

bool playerIDCompare(Player* a, Player* b) { return a->getID() < b->getID(); }

void Game::sortPlayers(std::vector<Player*>* players)
{
	sort(players->begin(), players->end(), playerIDCompare);
}

Game::Game(unsigned maxTurnNumber, unsigned boardSize, Coordinate chest)
	: board(boardSize, &players, chest), maxTurnNumber(maxTurnNumber), turnNumber(0)
{
}

Game::~Game()
{
	for (auto & player : players)
		delete player;
}

void Game::addPlayer(int id, int x, int y, Team team, std::string cls)
{
	if (cls == "ARCHER")
		players.push_back(new Archer(id, x, y, team));
	else if (cls == "TANK")
		players.push_back(new Tank(id, x, y, team));
	else if (cls == "PRIEST")
		players.push_back(new Priest(id, x, y, team));
	else if (cls == "FIGHTER")
		players.push_back(new Fighter(id, x, y, team));
	else if (cls == "SCOUT")
		players.push_back(new Scout(id, x, y, team));
	else
		return;

	if (team == BARBARIANS)
		barbars.push_back(players.back());
	else
		knights.push_back(players.back());

	sortPlayers(&players);
	sortPlayers(&barbars);
	sortPlayers(&knights);
}

bool Game::isGameEnded()
{	
	int barbarianCount = 0, knightCount = 0;

	for (auto player : players) {
		if (barbarianCount && knightCount)
			break;

		if (player->getTeam() == BARBARIANS)
			barbarianCount++;
		else
			knightCount++;
	}

	if (barbarianCount == 0) {
		std::cout << "Game ended at turn " << turnNumber << ". All barbarians dead. Knight victory." << std::endl;
		return true;
	}
	if (knightCount == 0) {
		std::cout << "Game ended at turn " << turnNumber << ". All knights dead. Barbarian victory." << std::endl;
		return true;
	}

	if (board.operator[](board.getChestCoordinates()) && board.operator[](board.getChestCoordinates())->getTeam() == BARBARIANS) {
		std::cout << "Game ended at turn " << turnNumber << ". Chest captured. Barbarian victory." << std::endl;
		return true;
	}

	if (turnNumber >= maxTurnNumber) {
		std::cout << "Game ended at turn " << maxTurnNumber
			<< ". Maximum turn number reached. Knight victory." << std::endl;
		return true;
	}

	return false;
}

void Game::playTurn()
{
	turnNumber++;
	std::cout << "Turn " << turnNumber << " has started." << std::endl;

	int playersSize = players.size();

	for (unsigned i = 0; i < players.size();) {
		playTurnForPlayer(players[i]);
		if (players.size() < (unsigned)playersSize) {
			playersSize--;
			continue;
		}
		i++;
	}
}

Goal Game::playTurnForPlayer(Player * player)
{

	if (player->isDead()) {
		if (player->getTeam() == KNIGHTS)
			knights.erase(std::find(knights.begin(), knights.end(), player));
		else
			barbars.erase(std::find(barbars.begin(), barbars.end(), player));

		std::cout << "Player " << player->getBoardID() << " has died." << std::endl;
		delete player;
		players.erase(std::find(players.begin(), players.end(), player));

		return NO_GOAL;
	}

	auto goals = player->getGoalPriorityList();
	std::vector<Coordinate> coordinates;
	Team playerTeam = player->getTeam();

	Goal result = NO_GOAL;

	for (Goal goal : goals) {
		if (result != NO_GOAL)
			break;

		if (goal == ATTACK) {
			coordinates = player->getAttackableCoordinates();
			Player* best = NULL;
			int smallest_id = INT_MAX;

			for (auto enemy : (player->getTeam() == KNIGHTS) ? barbars : knights) {
				if (std::find(coordinates.begin(), coordinates.end(), enemy->getCoord()) != coordinates.end() && enemy->getID() < (unsigned)smallest_id) {
					smallest_id = enemy->getID();
					best = enemy;
					result = ATTACK;
				}
			}

			if(best)
				player->attack(best);

			continue;
		}

		if (goal == CHEST) {
			coordinates = player->getMoveableCoordinates();
			Coordinate chest(board.getChestCoordinates().x, board.getChestCoordinates().y);
			Coordinate best(-1, -1);
			int base_dist = player->getCoord() - chest;
			int best_dist = base_dist;

			for (auto c : coordinates) {
				if (board.isCoordinateInBoard(c) && !(board.operator[](c)))
				{
					if (c - chest < best_dist) {
						best_dist = c - chest;
						best.x = c.x; best.y = c.y;
						result = CHEST;
					}
					else if (c - chest == best_dist && best_dist != base_dist) {
						best.x = (c.y - player->getCoord().y == 0) ? c.x : best.x;
						best.y = (c.y - player->getCoord().y == 0) ? c.y : best.y;
						best_dist = (c.y - player->getCoord().y == 0) ? c - chest : best_dist;
						result = CHEST;
					}
				}
			}

			if (best != Coordinate(-1, -1))
				player->movePlayerToCoordinate(best);

			continue;
		}

		if (goal == TO_ENEMY) {
			if ((player->getTeam() == KNIGHTS) ? barbars.size() == 0 : knights.size() == 0)
				continue;

			coordinates = player->getMoveableCoordinates();
			Coordinate enemyCoord = (player->getTeam() == KNIGHTS) ? barbars[0]->getCoord() : knights[0]->getCoord();
			int best_dist = enemyCoord - player->getCoord();

			for (auto enemy : (player->getTeam() == KNIGHTS) ? barbars : knights) {
				if (enemy->getCoord() - player->getCoord() < best_dist) {
					best_dist = enemy->getCoord() - player->getCoord();
					enemyCoord = enemy->getCoord();
				}
			}

			Coordinate best(-1,-1);
			int base_dist = player->getCoord() - enemyCoord;
			best_dist = base_dist;

			for (auto c : coordinates) {
				if (board.isCoordinateInBoard(c) && !(board.operator[](c)))
				{
					if (c - enemyCoord < best_dist) {
						best_dist = c - enemyCoord;
						best.x = c.x; best.y = c.y;
						result = TO_ENEMY;
					}
					else if (c - enemyCoord == best_dist && best_dist != base_dist) {
						best.x = (c.y - player->getCoord().y == 0) ? c.x : best.x;
						best.y = (c.y - player->getCoord().y == 0) ? c.y : best.y;
						best_dist = (c.y - player->getCoord().y == 0) ? c - enemyCoord : best_dist;
						result = TO_ENEMY;
					}
				}
			}

			if (best != Coordinate(-1, -1))
				player->movePlayerToCoordinate(best);

			continue;
		}

		if (goal == TO_ALLY) {
			if ((player->getTeam() == KNIGHTS) ? knights.size() == 0 : barbars.size() == 0)
				continue;

			coordinates = player->getMoveableCoordinates();
			Coordinate allyCoord = (player->getTeam() == KNIGHTS) ? knights[0]->getCoord() : barbars[0]->getCoord();
			int best_dist = allyCoord - player->getCoord();

			for (auto ally : (player->getTeam() == KNIGHTS) ? knights : barbars) {
				if (ally != player  && ally->getCoord() - player->getCoord() < best_dist) {
					best_dist = ally->getCoord() - player->getCoord();
					allyCoord = ally->getCoord();
				}
			}

			Coordinate best(-1, -1);
			int base_dist = player->getCoord() - allyCoord;
			best_dist = base_dist;

			for (auto c : coordinates) {
				if (board.isCoordinateInBoard(c) && !(board.operator[](c)))
				{
					if (c - allyCoord < best_dist) {
						best_dist = c - allyCoord;
						best.x = c.x; best.y = c.y;
						result = TO_ENEMY;
					}
					else if (c - allyCoord == best_dist && best_dist != base_dist) {
						best.x = (c.y - player->getCoord().y == 0) ? c.x : best.x;
						best.y = (c.y - player->getCoord().y == 0) ? c.y : best.y;
						best_dist = (c.y - player->getCoord().y == 0) ? c - allyCoord : best_dist;
						result = TO_ENEMY;
					}
				}
			}

			if (best != Coordinate(-1, -1))
				player->movePlayerToCoordinate(best);

			continue;
		}

		if (goal == HEAL) {
			coordinates = player->getHealableCoordinates();
			if (playerTeam == BARBARIANS) {
				for (auto ally : barbars) {
					if (std::find(coordinates.begin(), coordinates.end(), ally->getCoord()) != coordinates.end()) {
						player->heal(ally);
						result = HEAL;
					}
				}
			}
			else if (playerTeam == KNIGHTS) {
				for (auto ally : knights) {
					if (std::find(coordinates.begin(), coordinates.end(), ally->getCoord()) != coordinates.end()) {
						player->heal(ally);
						result = HEAL;
					}
				}
			}
			continue;
		}

	}
	return result;
}
