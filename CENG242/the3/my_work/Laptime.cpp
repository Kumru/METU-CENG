#include "Laptime.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Laptime::Laptime(int laptime)
	:laptime(laptime), next(NULL)
{
}

Laptime::Laptime(const Laptime & rhs)
{
	laptime = rhs.laptime;
	next = NULL;
}

Laptime::~Laptime()
{
}

void Laptime::addLaptime(Laptime * next)
{
	Laptime* tmp = this;
	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = next;
}

bool Laptime::operator<(const Laptime & rhs) const
{
	return laptime < rhs.laptime;
}

bool Laptime::operator>(const Laptime & rhs) const
{
	return laptime > rhs.laptime;
}

Laptime & Laptime::operator+(const Laptime & rhs)
{
	laptime += rhs.laptime;
	return *this;
}

std::ostream & operator<<(std::ostream & os, const Laptime & laptime)
{
	int tmpLaptime = laptime.laptime;
	int min, sec;
	
	std::string strsec, strmil;

	min = tmpLaptime / 60000;
	tmpLaptime -= min * 60000;
	
	sec = tmpLaptime / 1000;
	tmpLaptime -= sec * 1000;

	strsec = std::to_string(sec);
	if (sec < 10)
		strsec = "0" + strsec;

	strmil = std::to_string(tmpLaptime);
	if (tmpLaptime < 10)
		strmil = "00" + strmil;
	else if (tmpLaptime < 100)
		strmil = "0" + strmil;

	os << min << ":" << strsec << "." << strmil;
	return os;
}
