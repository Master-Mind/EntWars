#include "Time.h"

#include "SFML/System/Clock.hpp"

static TimeStruct time_;

const TimeStruct& Time()
{
	return time_;
}

void TimeUpdate()
{
	static sf::Clock clock;
	float tim = clock.restart().asSeconds();
	time_.dt = tim * time_.dtScale;
	time_.realdt = tim;
}

void SetTimeScale(float scale)
{
	time_.dtScale = scale;
}
