#include "GameManager.h"
#include "../Engine/HtCamera.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/ErrorLogger.h"
#include "../Engine/HtKeyboard.h"
#include <SDL.h>
#include <iomanip> // for std::setw
#include <sstream> // for std::ostringstream

GameManager GameManager::instance;
const int LEVEL_TIMER = 120;
const std::string LEVEL1_CONFIG = "Program/assets/LevelOne_conf.csv";

GameManager::GameManager()
{
	
}

GameManager::~GameManager()
{
}
	

void GameManager::StartGame()
{
	//Reset the player stats
	m_playerScore = 0;
	m_playerDamage = 0;
	m_playerAmmunition = 100;
	m_playerFuel = 100;
	m_playerKilled = false;
	m_currentLevel = nullptr;
	m_playerHasObject = false;
	m_playerIsFinished = false;
	

	//Load the level
	LoadLevel(1, LEVEL1_CONFIG);
}

void GameManager::EndGame()
{
	//delete the level memory
	delete m_currentLevel;
}

bool GameManager::LoadLevel(int levelNumber, const std::string& fileName)
{
	m_currentLevel = new Level(levelNumber, fileName);
	if (m_currentLevel && m_currentLevel->LoadLevel())
	{
		//Set the time for level completion
		m_levelTimer = Timer(LEVEL_TIMER);
		m_levelTimer.Start();
		return true;
	}
	ErrorLogger::Write("Unable to load the level from file: ");
	ErrorLogger::Write(fileName);
	return false;
}

bool GameManager::SetLevel(int levelNumber, const std::string& fileName)
{
	delete m_currentLevel;
	ObjectManager::instance.DeleteAllObjects();
	return LoadLevel(levelNumber, fileName);
}

void GameManager::SetPlayerKilled(bool isDead)
{
	m_playerKilled = isDead;
}

void GameManager::Update(double frameTime)
{
	m_levelTimer.Update();
	//This is the score and the timer rendered to stay on screen
	HtCamera::instance.UseCamera(false);
	if (m_playerHasObject)
	{
		DisplayObjectIcon();
	}

	if (m_playerIsFinished)
	{
		
		m_levelTimer.Stop();
		DisplayLevelComplete();
	}
	else
	{
		DisplayScore();
		DisplayTime();

		if (m_levelTimer.getTimeRemaining() == 0 || m_playerKilled)
		{
			DisplayGameOver();
		}
	}
		

	HtCamera::instance.UseCamera(true);
}

void GameManager::SetPlayerScore(int score)
{
	m_playerScore = score;
}

void GameManager::UpdateScore(int score)
{
	m_playerScore += score;
}

void GameManager::SetPlayerAmmunition(int ammunition)
{
	m_playerAmmunition = ammunition;
}

void GameManager::SetPlayerDamage(int damage)
{
	m_playerDamage = damage;
}

void GameManager::SetPlayerHasObject(int hasObject)
{
	m_playerHasObject = hasObject;
}

void GameManager::SetPlayerIsFinished(bool isFinished)
{
	m_playerIsFinished = true;
}


void GameManager::DisplayScore()
{
	HtGraphics::instance.WriteTextAligned(
		1000,
		1000,
		"Score: ",
		HtGraphics::WHITE,
		0,
		1.25
	);
	HtGraphics::instance.WriteIntAligned(
		1200,
		1000,
		m_playerScore,
		HtGraphics::WHITE,
		0,
		1.25
	);
}

void GameManager::DisplayTime()
{
	HtGraphics::instance.WriteTextAligned(
		1000,
		900,
		"Time: ",
		HtGraphics::WHITE,
		0,
		1.25
	);

	std::string timeFormatted = FormatTime(CalculateMinutesSeconds(m_levelTimer.getTimeRemaining()));
	HtGraphics::instance.WriteTextAligned(
		1200,
		900,
		timeFormatted,
		HtGraphics::WHITE,
		0,
		1.25
	);
}

void GameManager::DisplayGameOver()
{
	HtGraphics::instance.WriteTextCentered(
		Vector2D(0, 300),
		"Game Over",
		HtGraphics::WHITE,
		0,
		0.0,
		3.0
	);

	HtGraphics::instance.WriteTextCentered(
		Vector2D(0, 100),
		"Press Esc to go to the menu",
		HtGraphics::GREY,
		0,
		0.0,
		2.0
	);
	
}

void GameManager::DisplayLevelComplete()
{
	HtGraphics::instance.WriteTextCentered(
		Vector2D(0, 300),
		"Level Complete",
		HtGraphics::WHITE,
		0,
		0.0,
		5.0
	);

	int	finalScore = m_playerScore + (MAX_DAMAGE - m_playerDamage) + m_playerAmmunition + m_playerFuel + m_levelTimer.getTimeRemaining();
	
	HtGraphics::instance.WriteTextAligned(
		-660,
		150,
		"Final Score: ",
		HtGraphics::DARKGREEN,
		0,
		4.0
	);

	HtGraphics::instance.WriteIntAligned(
		300,
		150,
		finalScore,
		HtGraphics::DARKGREEN,
		0,
		4.0
	);

	HtGraphics::instance.WriteTextCentered(
		Vector2D(0, -200),
		"Press Esc to go to the menu",
		HtGraphics::GREY,
		0,
		0.0,
		3.0
	);
}

void GameManager::DisplayObjectIcon()
{
	PictureIndex objectIcon = HtGraphics::instance.LoadPicture("..\\Program\\assets\\GoalPickup.png");
	HtGraphics::instance.DrawAt(Vector2D(1100, 750),objectIcon);
	
}

void GameManager::HandleEvent(Event evt)
{

}

std::tuple<int, int> GameManager::CalculateMinutesSeconds(int timeSeconds)
{
	int minutes = timeSeconds / 60;
	int seconds = timeSeconds % 60;

	return std::make_tuple(minutes, seconds);
}

std::string GameManager::FormatTime(std::tuple<int, int> minutesSeconds)
{
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << std::get<0>(minutesSeconds) << ":"; // Format minutes with leading zero if necessary
	oss << std::setw(2) << std::setfill('0') << std::get<1>(minutesSeconds); // Format seconds with leading zero if necessary
	return oss.str();

}
