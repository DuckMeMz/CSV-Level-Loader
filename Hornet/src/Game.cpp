#include "Game.h"
#include "../Engine/HtCamera.h"
#include "../Engine/HtMouse.h"
#include "../Engine/ObjectManager.h"
#include "GameManager.h"
//#include "Entities.h"

void Game::StartOfProgram()
{
    // This makes the operating system's mouse pointer invisible
    // Usually best to use your own.
    //HtMouse::instance.SetPointerVisiblity(false);
}

void Game::StartOfGame()
{
    GameManager::instance.StartGame();

}

void Game::Update(double frametime)
{
    m_timer.Mark();
    
    ObjectManager::instance.UpdateAll(frametime);
    ObjectManager::instance.ProcessCollisions();
    ObjectManager::instance.RenderAll();
    GameManager::instance.Update(frametime);
    //GameManager::instance.RenderGUI();
#ifdef _DEBUG
    ObjectManager::instance.CycleDebugObject();
    ObjectManager::instance.RenderDebug();
#endif
    ObjectManager::instance.DeleteInactiveItems();

    // Draws all graphics to the screen. Best not to remove this line.
    HtGraphics::instance.Present();
}

void Game::OnSuspend()
{
    HtAudio::instance.StopAllChannels();
}

void Game::OnResume()
{

}

void Game::EndOfGame()
{
    GameManager::instance.EndGame();
    ObjectManager::instance.DeleteAllObjects();
}

void Game::EndOfProgram()
{
}
