#include "Timer.h"

Timer::Timer(int duration) : 
    m_duration(duration), 
    m_startTime(0), 
    m_timeRemaining(duration), 
    m_isRunnung(false)
{ }

void Timer::Start()
{
    m_isRunnung = true;
    m_startTime = SDL_GetTicks();
}

void Timer::Stop()
{
    m_isRunnung = false;
}

void Timer::Update()
{
    if (m_isRunnung)
    {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - m_startTime;
        m_timeRemaining = m_duration - (elapsedTime / 1000); //convert from milliseconds to seconds

        if (m_timeRemaining <= 0)
        {
            m_timeRemaining = 0;
            Stop();
        }
    }
}

int Timer::getTimeRemaining() const
{
    return m_timeRemaining;
}
