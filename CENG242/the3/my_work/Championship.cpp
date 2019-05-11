#include "Championship.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

std::string zeroes(int n, int basamak);

bool compare(std::pair<int, std::string> fst, std::pair<int, std::string> scd) {
	return fst.first > scd.first;
}

Championship::Championship()
{
}

Championship::Championship(const Championship & rhs)
{
	for (auto race : rhs.races)
		races.push_back(new Race(*race));
}

Championship::~Championship()
{
	for (auto race : races) {
		delete race;
	}
}

void Championship::addNewRace(Race & race)
{
	races.push_back(new Race(race));
	
	Car* tmpOld = race.getHead();
	Car* tmpNew = races[0]->getHead();

	while (tmpNew && tmpOld) {
		Laptime* tmp = tmpOld->getHead();
		if (tmp) {
			tmpNew->setHead(new Laptime(*tmp));
			tmp = tmp->getNext();
		}
		
		while (tmp) {
			tmpNew->getHead()->addLaptime(new Laptime(*tmp));
			tmp = tmp->getNext();
		}

		tmpOld = tmpOld->getNext();
		tmpNew = tmpNew->getNext();
	}
}

void Championship::addNewRace(std::string race_name)
{
	races.push_back(new Race(*(races[0])));
	races.back()->setName(race_name);
	races.back()->getLaptime().setLaptime(Utilizer::generateAverageLaptime());
}

void Championship::removeRace(std::string race_name)
{
	int pos = 0;
	Race* toDel = NULL;

	for (unsigned i = 0; i < races.size(); i++) {
		if (races[i]->getRaceName() == race_name) {
			pos = i; toDel = races[i];  break;
		}
	}
	delete races[pos];
	races.erase(races.begin()+pos);
}

void Championship::addLap(std::string race_name)
{
	Race* tmp = races[0];
	for (auto race : races) {
		if (race->getRaceName() == race_name) {
			tmp = race; break;
		}
	}

	++(*tmp);
}

Race& Championship::operator[](std::string race_name)
{
	Race* tmp = races[0];
	for (auto & race : races) {
		if (race->getRaceName() == race_name) {
			tmp = race; break;
		}
	}

	return *tmp;
}

std::ostream & operator<<(std::ostream & os, const Championship & championship)
{
	int carCount = championship.races[0]->getNumberOfCarsinRace();

	std::unordered_map<std::string, int> points;
	//names
	Car* tmp = championship.races[0]->getHead();
	while (tmp) {
		points.insert({tmp->getThreeLetters(), 0});
		tmp = tmp->getNext();
	}

	for (auto race : championship.races) {
		Car* tmp = race->getHead();
		Car* fastest = race->getHead();

		int i = 1;
		while (tmp) {
			if (tmp->getFastest()->getLaptime() < fastest->getFastest()->getLaptime())
				fastest = tmp;

			switch (i++) {
				case 1: points[tmp->getThreeLetters()] += 25; break;
				case 2: points[tmp->getThreeLetters()] += 18; break;
				case 3: points[tmp->getThreeLetters()] += 15; break;
				case 4: points[tmp->getThreeLetters()] += 12; break;
				case 5: points[tmp->getThreeLetters()] += 10; break;
				case 6: points[tmp->getThreeLetters()] += 8; break;
				case 7: points[tmp->getThreeLetters()] += 6; break;
				case 8: points[tmp->getThreeLetters()] += 4; break;
				case 9: points[tmp->getThreeLetters()] += 2; break;
				case 10: points[tmp->getThreeLetters()] += 1; break;
				default: points[tmp->getThreeLetters()] += 0; break;
			}

			tmp = tmp->getNext();
		}

		points[fastest->getThreeLetters()] += 1;

	}

	std::vector<std::pair<int, std::string>> scoreB;
	scoreB.reserve(carCount);

	for (auto item : points) {
		scoreB.push_back({item.second, item.first});
		//std::cout << item.first << " " << item.second << std::endl;
	}

	sort(scoreB.begin(), scoreB.end(), compare);

	int digits = 0;
	int cC = carCount;
	do {
		cC /= 10;
		digits++;
	} while (cC != 0);

	int i = 1;
	for (auto item : scoreB) {
		os << zeroes(i++, digits) << "--" << item.second << "--" << item.first << std::endl;
	}

	return os;
}
