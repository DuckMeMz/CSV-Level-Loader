#include "HornetMenus.h"
#include "HtKeyboard.h"
#include "../src/HornetApp.h"
#include "ErrorLogger.h"
#include "HtCamera.h"
#include <fstream>

void HornetMenus::Initialise()
{

    m_currentOption = 0;
    m_currentType = MenuType::MAIN;

    m_mainOptionText.push_back("Start game");
    m_mainOptionText.push_back("Credits");
    m_mainOptionText.push_back("Exit");

    m_pauseOptionText.push_back("Resume");
    m_pauseOptionText.push_back("Exit");

    std::ifstream file;
    file.open(DOCS+"credits.txt");
    if (!file.is_open())
    {
        ErrorLogger::Write("credits.txt not found in "+DOCS+" folder.");
    }
    else
    {
        std::string line;
        while (std::getline(file, line)) 
        {
            m_creditsText.push_back(line);
        }
    }
    file.close();

    m_clickSound= HtAudio::instance.LoadSound(ASSETS + "buttonClick.wav");
    m_selectSound = HtAudio::instance.LoadSound(ASSETS + "buttonChoice.wav");;
}

void HornetMenus::Update(double frameTime, HornetApp* pOwner)
{
    if (m_currentType == MenuType::MAIN)
    {
        UpdateMain(frameTime, pOwner);
    }
    else if (m_currentType == MenuType::PAUSE)
    {
        UpdatePause(frameTime, pOwner);
    }
    else if (m_currentType == MenuType::CREDITS)
    {
        UpdateCredits(frameTime, pOwner);
    }

    HtGraphics::instance.Present();
}

void HornetMenus::Shutdown()
{
    m_creditsText.clear();
    m_pauseOptionText.clear();
    m_mainOptionText.clear();
}

void HornetMenus::SwitchToPause()
{
    m_currentType = MenuType::PAUSE;
}

void HornetMenus::SwitchToMain()
{
    m_currentType = MenuType::MAIN;
}

void HornetMenus::SwitchToCredits()
{
    m_currentType = MenuType::CREDITS;
    m_creditFlowTimer = 0;
}

void HornetMenus::DisplayLoadScreen()
{
    HtGraphics::instance.Present();
    HtGraphics::instance.WriteTextAligned(Vector2D(-200, 50), "LOADING",
        HtGraphics::RED, 4, 3.0);

    HtGraphics::instance.Present();
}

HornetMenus::HornetMenus()
{
    m_clickSound = HtAudio::NOSOUND;
    m_selectSound = HtAudio::NOSOUND;
    m_creditFlowTimer = 0;
    m_currentOption=0;
    m_currentType= MenuType::MAIN;
}

HornetMenus::~HornetMenus()
{
}

void HornetMenus::UpdateMain(double frameTime, HornetApp* pOwner)
{
    bool cameraActive = HtCamera::instance.IsCameraActive();
    HtCamera::instance.UseCamera(false);

    HtGraphics::instance.WriteTextAligned(Vector2D(-1000, 750), APPNAME, HtGraphics::RED, 0, 2.0);

    for (int i = 0; i < m_mainOptionText.size(); ++i)
    {
        if (i == m_currentOption)
        {
            HtGraphics::instance.WriteTextAligned(Vector2D(-1000.0, double(625.0-120.0*i)), m_mainOptionText[i],
                HtGraphics::BLUE, 0, 2.4f);
        }
        else
        {
            HtGraphics::instance.WriteTextAligned(Vector2D(-1000.0, double(600.0-120.0 * i)), m_mainOptionText[i],
                HtGraphics::LIGHTBLUE, 0, 2.0);
        }
    }

    // Handle player choice
    static bool input = true;
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_UP))
    {
        if (input == false)
        {
            HtAudio::instance.Play(m_clickSound);
            m_currentOption--;
            if (m_currentOption < 0)
                m_currentOption = (int)m_mainOptionText.size()-1;
        }
        input = true;
    }
    else if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_DOWN))
    {
        if (input == false)
        {
            HtAudio::instance.Play(m_clickSound);
            ++m_currentOption;
            if (m_currentOption>= m_mainOptionText.size())
                m_currentOption = 0;
        }
        input = true;
    }
    else if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_RETURN))
    {
        if (input == false)
        {
            HtAudio::instance.Play(m_selectSound);
            // Handle option choice
            if (m_currentOption == 0)
                pOwner->SetState(HtProgramState::GAMEPLAY);
            else if (m_currentOption == 1)
                SwitchToCredits();
            else if (m_currentOption == 2)
                pOwner->SetState(HtProgramState::QUIT);
        }
        input = true;
    }
    else
    {
        input = false;
    }
    HtCamera::instance.UseCamera(cameraActive);
}

void HornetMenus::UpdatePause(double frametime, HornetApp* pOwner)
{
    bool cameraActive = HtCamera::instance.IsCameraActive();
    HtCamera::instance.UseCamera(false);
    HtGraphics::instance.WriteTextAligned(Vector2D(-1000, 750), "PAUSED", HtGraphics::RED, 0, 2.0);

    for (int i = 0; i < m_pauseOptionText.size(); ++i)
    {
        if (i == m_currentOption)
        {
            HtGraphics::instance.WriteTextAligned(Vector2D(-1000.0, 625.0 - double(120.0 * i)), m_pauseOptionText[i],
                HtGraphics::BLUE, 0, 2.4f);
        }
        else
        {
            HtGraphics::instance.WriteTextAligned(Vector2D(-1000.0, 600.0 - double(120.0 * i)), m_pauseOptionText[i],
                HtGraphics::LIGHTBLUE,0, 2.0);
        }
    }

    // Handle player choice
    static bool input = true;
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_UP))
    {
        if (input == false)
        {
            HtAudio::instance.Play(m_clickSound);
            m_currentOption--;
            if (m_currentOption < 0)
                m_currentOption = (int)m_pauseOptionText.size()-1;
        }
        input = true;
    }
    else if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_DOWN))
    {
        if (input == false)
        {
            HtAudio::instance.Play(m_clickSound);
            ++m_currentOption;
            if (m_currentOption >= m_pauseOptionText.size())
                m_currentOption = 0;
        }
        input = true;
    }
    else if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_RETURN))
    {
        if (input == false)
        {
            // Handle option choice
            if (m_currentOption == 0)
            {
                pOwner->SetState(HtProgramState::GAMEPLAY);
                HtAudio::instance.Play(m_selectSound);
            }

            else if (m_currentOption == 1)
            {
                pOwner->SetState(HtProgramState::MAINMENU);
                HtAudio::instance.Play(m_selectSound);
            }
        }
        input = true;
    }
    else
    {
        input = false;
    }
    HtCamera::instance.UseCamera(cameraActive);
}

void HornetMenus::UpdateCredits(double frametime, HornetApp* pOwner)
{
    bool cameraActive = HtCamera::instance.IsCameraActive();
    HtCamera::instance.UseCamera(false);
    const int MAXLINES = 20;

    m_creditFlowTimer += frametime;
    HtGraphics::instance.WriteTextAligned(Vector2D(-1200.0, 800.0), "CREDITS", HtGraphics::RED, 0, 2.0);
    HtGraphics::instance.WriteTextAligned(Vector2D(600.0, -900.0), "Enter to return", HtGraphics::RED, 0, 2.0);

    int currentRow = int(m_creditFlowTimer);
    int part = int((m_creditFlowTimer - currentRow) * 50.0);

    if (currentRow - MAXLINES > int(m_creditsText.size()))
    {
        m_creditFlowTimer = 0;
    }
    for (int i = 0; i < m_creditsText.size(); ++i)
    {

        if (i < currentRow && i>currentRow-MAXLINES)
        {
            HtGraphics::instance.WriteTextAligned(Vector2D(-1200.0, -600- 50 * (double(i) - m_creditFlowTimer)), m_creditsText[i],
                HtGraphics::WHITE, 2, 1.0);
        }
        else if (i == currentRow)
        {
            std::string text = m_creditsText[i];// .substr(0, part * 2);
            for (char& c : text)
            {
                if (c<80-part || c>80+part)
                    c = ' ';
            }
            Colour c(5 * part, 255, 255, 255);
            HtGraphics::instance.WriteTextAligned(Vector2D(-1200.0, -600-50 * (double(i) - m_creditFlowTimer)), text,
                c, 2, 1.0);
        }
        else if (i == currentRow - MAXLINES)
        {
            int oppart = 50 - part;
            std::string text = m_creditsText[i];
            for (char& c : text)
            {
                if (c < 80 - oppart || c>80 + oppart)
                    c = '*';
            }
            Colour c(255 - 5 * part, 255, 255, 255);
            HtGraphics::instance.WriteTextAligned(Vector2D(-1200.0, -600 - 50 * (double(i) - m_creditFlowTimer)), text,
                c, 2, 1.0);
        }
    }
    // Handle player choice
    static bool input = true;
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_RETURN) ||
        HtKeyboard::instance.KeyPressed(SDL_SCANCODE_ESCAPE))
    {
        if (input == false)
        {
            // Handle option choice
            SwitchToMain();
            HtAudio::instance.Play(m_clickSound);
        }
        input = true;
    }
    else
    {
        input = false;
    }
    HtCamera::instance.UseCamera(cameraActive);
}
