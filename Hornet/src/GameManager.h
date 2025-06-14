#pragma once
#include "../Engine/ObjectManager.h"
#include "../Engine/HornetMenus.h"
#include "../Engine/Timer.h"
#include <tuple>
#include "level.h"

class GameManager
{
private:
	// Sets up the object and sets the currentScene to zero
	// Also sets the gametimer
	GameManager();
	// Deletes all objects
	~GameManager();
	GameManager(const GameManager& other) = delete;
	GameManager(GameManager&& other) = delete;
	
	void DisplayScore();
	void DisplayTime();
	void DisplayGameOver();
	void DisplayLevelComplete();
	void DisplayObjectIcon();

	std::tuple<int, int> CalculateMinutesSeconds(int timeSeconds);
	std::string FormatTime(std::tuple<int, int> minutesSeconds);

	
	
	int m_playerScore;
	int m_playerDamage;
	int m_playerAmmunition;
	int m_playerFuel;
	bool m_playerKilled;
	Level* m_currentLevel;
	Timer m_levelTimer;
	bool m_playerHasObject;
	bool m_playerIsFinished;
	

public:
	static GameManager instance;
	
	void StartGame();
	void EndGame();
	bool LoadLevel(int levelNumber, const std::string& fileName);
	bool SetLevel(int levelNumber, const std::string& fileName);
	
	void Update(double frameTime);
	void UpdateScore(int score);
	void SetPlayerKilled(bool isDead);
	void SetPlayerScore(int score);
	void SetPlayerAmmunition(int ammunition);
	void SetPlayerDamage(int damage);
	void SetPlayerHasObject(int hasObject);
	void SetPlayerIsFinished(bool isFinished);

	void HandleEvent(Event evt);

	int GetTimeRemaining() const { return m_levelTimer.getTimeRemaining(); }

	const int MAX_FUEL = 100;
	const int MAX_DAMAGE = 100;
	const int MAX_AMMUNITION = 100;
};

