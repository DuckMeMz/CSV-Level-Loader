#pragma once
#include <SDL.h>

class Timer
{
public:
	Timer() {};
	Timer(int duration);

	void Start();
	void Stop();
	void Update();

	int getTimeRemaining() const;

private:
	bool m_isRunnung;
	Uint32 m_startTime;
	int m_duration; //this is in seconds
	int m_timeRemaining; //this is in seconds
};

