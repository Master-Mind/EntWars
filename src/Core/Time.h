#pragma once

struct TimeStruct
{
	float dt = 0;
	float realdt = 0;
	float dtScale = 0;
};

const TimeStruct& Time();


void TimeUpdate();
void SetTimeScale(float scale);