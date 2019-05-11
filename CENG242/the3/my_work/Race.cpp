#include "Race.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

std::string zeroes(int n, int basamak) {
	int digit = 0, tmpN = n;
	do {
		tmpN /= 10;
		digit++;
	} while (tmpN != 0);

	std::string res = "";
	for (int i = 0; i < basamak - digit; i++)
		res += '0';
	res += std::to_string(n);

	return res;
}

void Race::swap(Car* head, int index){
	
	if (index == 0) {
		Car* tmp = head->getNext();
		head->setNext(tmp->getNext());
		tmp->setNext(head);
		this->head = tmp;
		
		return;
	}

	Car* fst = this->head;
	Car* scd = this->head;

	for (int i = 1; i < index  ; i++)
		fst = fst->getNext();
	scd = fst->getNext()->getNext();

	Car* tmp = scd->getNext();
	fst->getNext()->setNext(tmp);
	scd->setNext(fst->getNext());
	fst->setNext(scd);
}

void Race::sort(){

	int carCount = getNumberOfCarsinRace();

	if (carCount < 2)
		return;

	Car *fst = head, *scd = fst->getNext();

	for (int i = 0; i < carCount; i++) {
		for (int j = 0; j < carCount - i - 1; j++) {
			if (*fst > *scd) {
				swap(fst, j);

				scd = fst->getNext();
				continue;
			}
			fst = fst->getNext();
			scd = fst->getNext();
		}
		fst = head;
		scd = fst->getNext();
	}
}

Race::Race(std::string race_name)
	:race_name(race_name), average_laptime(Utilizer::generateAverageLaptime()), head(NULL)
{
}

Race::Race(const Race & rhs)
	: race_name(rhs.race_name), average_laptime(rhs.average_laptime), head(NULL)
{
	Car* tmp = rhs.head;
	if (!head && tmp) {
		head = new Car(*tmp);
		tmp = tmp->getNext();
	}
	while (tmp) {
		Car* copy = new Car(*tmp);
		head->addCar(copy);
		tmp = tmp->getNext();
	}

	goBacktoLap(-1);
}

Race::~Race()
{
	Car* toDel = head, *curr = head;
	while (curr) {
		curr = curr->getNext();
		delete toDel;
		toDel = curr;
	}
}

std::string Race::getRaceName() const
{
	return race_name;
}

void Race::addCartoRace()
{
	static int nameId = 0;
	Car* tmp = new Car("Vabis " + std::to_string(nameId) + "Bebek");
	if (!head)
		head = tmp;
	else
		head->addCar(tmp);
	nameId++;
}

void Race::addCartoRace(Car & car)
{
	Car* tmp = new Car(car);
	if (!head)
		head = tmp;
	else
		head->addCar(tmp);
}

int Race::getNumberOfCarsinRace()
{
	int counter = 0;
	Car* tmp = head;
	while (tmp) {
		counter++;
		tmp = tmp->getNext();
	}

	return counter;
}

void Race::goBacktoLap(int lap)
{
	Car* currCar = head;
	while (currCar) {
		Laptime* currLap = currCar->getHead();
		int index = 0;
		while (index++ < lap) {
			currLap = currLap->getNext();
		}

		Laptime* toDel = currLap;
		if (lap != -1) {
			currLap = currLap->getNext();
			toDel->setNext(NULL);
			toDel = currLap;
		}
		else if (currLap) {
			currLap = currLap->getNext();
			delete toDel;
			toDel = currLap;
			currCar->setHead(NULL);
		}

		while (currLap) {
			currLap = currLap->getNext();
			delete toDel;
			toDel = currLap;
		}
		currCar = currCar->getNext();
	}

	sort();
}

void Race::operator++()
{
	Car* tmp = head;
	while (tmp) {
		tmp->Lap(average_laptime);
		tmp = tmp->getNext();
	}

	sort();
}

void Race::operator--()
{
	int lapcount = 0;
	Laptime* tmp = head->getHead();
	while (tmp) {
		lapcount++;
		tmp = tmp->getNext();
	}
	goBacktoLap(lapcount - 1);

	sort();
}

Car Race::operator[](const int car_in_position)
{
	Car* tmp = head;
	
	for (int i = 0; i < car_in_position; i++) {
		tmp = tmp->getNext();
	}

	return *tmp;
}

Car Race::operator[](std::string driver_name)
{
	Car* tmp = head;

	while(driver_name != tmp->getDriverName()){
		tmp = tmp->getNext();
	}

	return *tmp;
}

Race & Race::operator=(const Race & rhs)
{
	race_name = rhs.race_name;
	head = rhs.head;
	average_laptime = rhs.average_laptime;
	return *this;
}

std::ostream & operator<<(std::ostream & os, const Race & race)
{
	int carCount = 0;
	Car* tmp = race.head, *fastest = race.head;
	while (tmp) {
		carCount++;
		if (tmp->getFastest()->getLaptime() < fastest->getFastest()->getLaptime())
			fastest = tmp;
		tmp = tmp->getNext();
	}

	//how many digits of cars
	int digits = 0;
	int cC = carCount;
	do {
		cC /= 10;
		digits++;
	} while (cC != 0);

	int i = 1;
	tmp = race.head;
	while (tmp) {
		os << zeroes(i, digits) << "--" << *tmp << "--";
		switch (i) {
			case 1: os << 25; break;
			case 2: os << 18; break;
			case 3: os << 15; break;
			case 4: os << 12; break;
			case 5: os << 10; break;
			case 6: os << 8; break;
			case 7: os << 6; break;
			case 8: os << 4; break;
			case 9: os << 2; break;
			case 10: os << 1; break;
			default: os << 0; break;
		}
		if (fastest == tmp && i < 11)
			os << "--" << 1;

		if(i!=carCount)
			os << std::endl;
		tmp = tmp->getNext();
		i++;
	}
	return os;
}
