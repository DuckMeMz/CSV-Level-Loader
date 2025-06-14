#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2main")
#include "sdl.h"
#include <fstream>
#include "HornetApp.h"
#include "../Engine/HtGraphics.h"
#include "../Engine/HtAudio.h"
#include "../Engine/HtKeyboard.h"
#include "../Engine/HtMouse.h"
#include "../Engine/GameTimer.h"
#include "Game.h"
#include "../Engine/HornetMenus.h"
#include "../Engine/HtGameController.h"
#include "../Engine/ErrorLogger.h"



Result HornetApp::Initialise()
{
   // SDL_Window* pWindow= nullptr;
    // Initialises SDL. If this fails, we are in big trouble
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
    {
        ErrorLogger::Write("Failed to initialise SDL in Initialise().");
        ErrorLogger::Write(SDL_GetError());
        return Result::FAILURE;
    }

    //// Initialise the sound engine
    if (HtAudio::instance.Initialise() == Result::FAILURE)
    {
        ErrorLogger::Write("Failed to initialise HtAudio in Initialise().");
        ErrorLogger::Write(SDL_GetError());
        return Result::FAILURE;
    }

    //// Initialise the draw engine
    if (HtGraphics::instance.Initialise() == Result::FAILURE)
    {
        ErrorLogger::Write("Failed to initialise HtGraphics in Initialise().");
        ErrorLogger::Write(SDL_GetError());
        return Result::FAILURE;
    }

    //// Initialise the keyboard engine
    if (HtKeyboard::instance.Initialise() == Result::FAILURE)
    {
        ErrorLogger::Write("Failed to initialise HtKeyboard in Initialise().");
        ErrorLogger::Write(SDL_GetError());
        return Result::FAILURE;
    }

    //// Initialise the mouse engine
    if (HtMouse::instance.Initialise() == Result::FAILURE)
    {
        ErrorLogger::Write("Failed to initialise HtMouse in Initialise().");
        ErrorLogger::Write(SDL_GetError());
        return Result::FAILURE;
    }

    //// Initialise the controller, if any
    HtGameController::instance.Initialise();

    m_theGame.StartOfProgram();

    return Result::SUCCESS;
}

void HornetApp::HandleEvent(SDL_Event &evt)
{
    // If the event is an instruction to quit, do that
    if (evt.type == SDL_QUIT)
    {
        SetState(HtProgramState::QUIT);
    }
    else if (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP)
    {
        // Hard code the escape key. I know what students are like.
        if (evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE
            && GetState() == HtProgramState::GAMEPLAY)
        {
            SetState(HtProgramState::PAUSEMENU);
        }
        else
        {
            HtKeyboard::instance.HandleKeyEvent(evt);
        }
    }
    else if (evt.type == SDL_TEXTINPUT)
    {
        HtKeyboard::instance.AddTextInput(evt.text.text);
    }
    else if (evt.type == SDL_MOUSEMOTION
        || evt.type == SDL_MOUSEBUTTONDOWN
        || evt.type == SDL_MOUSEBUTTONUP
        || evt.type == SDL_MOUSEWHEEL)
    {
        HtMouse::instance.HandleMouseEvent(evt);

    }
}

HtProgramState HornetApp::GetState() const
{
    return m_programState;
}

void HornetApp::Tick()
{
    static GameTimer gt;
    //set the frame rate to 60 frames/second
    gt.SetMinimumFrameTime(0.0111);
    gt.SetMaximumFrameTime(0.0111);
    gt.Mark();

    if (GetState() == HtProgramState::MAINMENU
        || GetState() == HtProgramState::PAUSEMENU)
    {
        m_theMenus.Update(gt.frameTime, this);
    }
    else if (GetState() == HtProgramState::GAMEPLAY)
    {
        m_theGame.Update(gt.frameTime);
    }
}

void HornetApp::Shutdown()
{
    m_theGame.EndOfProgram();
    m_theMenus.Shutdown();

    HtAudio::instance.Shutdown();
    HtGraphics::instance.Shutdown();
    HtKeyboard::instance.Shutdown();

    // Exit SQL. This releases the dll and other resources. Do this last
    SDL_Quit();
}

void HornetApp::SetState(HtProgramState newState)
{
    switch (m_programState)
    {
    case HtProgramState::INITIALISE:
        if (newState == HtProgramState::MAINMENU)
        {
            m_theMenus.Initialise();
            m_theMenus.SwitchToMain();
        }
        break;
    case HtProgramState::MAINMENU:
        if (newState == HtProgramState::GAMEPLAY)
        {
            m_theMenus.DisplayLoadScreen();
            m_theGame.StartOfGame();
        }
        break;
    case HtProgramState::PAUSEMENU:
        if (newState == HtProgramState::MAINMENU)
        {
            m_theGame.EndOfGame();
            m_theMenus.SwitchToMain();
        }
        else if (newState == HtProgramState::GAMEPLAY)
        {
            m_theGame.OnResume();
        }
        break;
    case HtProgramState::QUIT:

        break;
    case HtProgramState::GAMEPLAY:
        if (newState == HtProgramState::PAUSEMENU)
        {
            m_theGame.OnSuspend();
            m_theMenus.SwitchToPause();
        }
        break;
    }
    m_programState = newState;
}