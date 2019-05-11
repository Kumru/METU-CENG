#include "Car.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Car::Car(std::string driver_name)
	:driver_name(driver_name), head(NULL), next(NULL), performance(Utilizer::generatePerformance())
{
}

Car::Car(const Car & rhs)
{
	driver_name = rhs.driver_name;
	performance = rhs.performance;
	next = NULL;
	head = NULL;
	
	//Laptime's
	Laptime* tmp = rhs.head;
	if (tmp) {
		head = new Laptime(*tmp);
		tmp = tmp->getNext();
	}
	while (tmp) {
		head->addLaptime(new Laptime(*tmp));
		tmp = tmp->getNext();
	}
}

Car::~Car()
{
	//clear laptimes
	Laptime* toDel = head, *curr = head;
	while (curr) {
		curr = curr->getNext();
		delete toDel;
		toDel = curr;
	}
}

std::string Car::getDriverName() const
{
	return driver_name;
}

double Car::getPerformance() const
{
	return performance;
}

void Car::addCar(Car * next)
{
	Car* tmp = this;
	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = next;
}

bool Car::operator<(const Car & rhs) const
{
	Laptime* tmp = head;
	int lhsSum = 0, rhsSum = 0;
	while (tmp) {
		lhsSum += tmp->getLaptime();
		tmp = tmp->getNext();
	}

	tmp = rhs.head;
	while (tmp) {
		rhsSum += tmp->getLaptime();
		tmp = tmp->getNext();
	}

	return lhsSum < rhsSum;
}

bool Car::operator>(const Car & rhs) const
{
	Laptime* tmp = head;
	int lhsSum = 0, rhsSum = 0;
	while (tmp) {
		lhsSum += tmp->getLaptime();
		tmp = tmp->getNext();
	}

	tmp = rhs.head;
	while (tmp) {
		rhsSum += tmp->getLaptime();
		tmp = tmp->getNext();
	}

	return lhsSum > rhsSum;
}

Laptime Car::operator[](const int lap) const
{
	Laptime* tmp = head;

	if (!tmp)
		return Laptime(0);

	for (int i = 0; i < lap; i++) {
		if (!tmp)
			return Laptime(0);
		tmp = tmp->getNext();
	}

	return *tmp;
}

void Car::Lap(const Laptime & average_laptime)
{
	int variance = Utilizer::generateLaptimeVariance(performance);
	Laptime* newLaptime = new Laptime(variance + average_laptime.getLaptime());
	if (!head)
		head = newLaptime;
	else
		head->addLaptime(newLaptime);
}

std::string Car::getThreeLetters() const
{
	int wsPlace = driver_name.find(" ");
	std::string res = "";

	res += toupper(driver_name[wsPlace + 1]);
	res += toupper(driver_name[wsPlace + 2]);
	res += toupper(driver_name[wsPlace + 3]);

	return res;
}

Laptime * Car::getFastest() const
{
	Laptime* tmp = head, *fastest = head;
	while (tmp) {
		if (*tmp < *fastest)
			fastest = tmp;
 		tmp = tmp->getNext();
	}

	return fastest;
}

std::ostream & operator<<(std::ostream & os, const Car & car)
{
	std::string name = car.getThreeLetters();
	Laptime* fastest = car.getFastest(), *tmp = car.head, *latest = car.head;
	int total = 0;
	while (tmp) {
		total += tmp->getLaptime();
		if (!tmp->getNext())
			latest = tmp;
		tmp = tmp->getNext();
	}

	Laptime totalLapt(total);

	os << name << "--" << *latest << "--" << *fastest << "--" << totalLapt;
	return os;
}
