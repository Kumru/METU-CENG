#include"Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Board::Board(unsigned _size, std::vector<Player*>* _players, Coordinate chest)
	:size(_size), players(_players), chest(chest)
{
}

Board::~Board()
{
}

bool Board::isCoordinateInBoard(const Coordinate & c)
{
	return ((c.x < size && c.x >= 0) && (c.y < size && c.y >= 0));
}

bool Board::isPlayerOnCoordinate(const Coordinate & c)
{
	for (auto player : *players)
		if (player->getCoord() == c)
			return true;

	return false;
}

Player * Board::operator[](const Coordinate & c)
{
	for (auto player : *players)
		if (player->getCoord() == c)
			return player;

	return NULL;
}

void Board::printBoardwithID()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (isPlayerOnCoordinate({j,i}))
				std::cout << this->operator[]({ j, i })->getBoardID() << " ";
			else if (getChestCoordinates().y == i && getChestCoordinates().x == j)
				std::cout << "Ch ";
			else
				std::cout << "__ ";
		}
		std::cout << std::endl;
	}
}

void Board::printBoardwithClass()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (isPlayerOnCoordinate({ j,i }))
				std::cout << this->operator[]({ j, i })->getClassAbbreviation() << " ";
			else if (getChestCoordinates().x == j && getChestCoordinates().y == i)
				std::cout << "Ch ";
			else
				std::cout << "__ ";
		}
		std::cout << std::endl;
	}
}
