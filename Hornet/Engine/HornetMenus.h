#pragma once
#include "HtGraphics.h"
#include "HtAudio.h"
#include <vector>

class HornetApp;

// This is a fairly crude class to manage a basic menu system.
// Currently includes the main menu, the credits menu and the pause menu.
// I will probably add settings menus later, but this class is almost a 
// placeholder.

// Modified 19/04/24
// Stopped menus from using the camera position

class HornetMenus
{
public:
    void Initialise();
    void Update(double frameTime, HornetApp* pOwner);
    void Shutdown();
    void SwitchToPause();
    void SwitchToMain();
    void SwitchToCredits();
    void DisplayLoadScreen();
    HornetMenus();
    ~HornetMenus();


private:
    enum class MenuType{MAIN, PAUSE, CREDITS};
    void UpdateMain(double frametime, HornetApp* pOwner);
    void UpdatePause(double frametime, HornetApp* pOwner);
    void UpdateCredits(double frametime, HornetApp* pOwner);
    int m_currentOption;
    double m_creditFlowTimer;
    MenuType m_currentType;
    std::vector<std::string> m_mainOptionText;
    std::vector<std::string> m_pauseOptionText;
    std::vector<std::string> m_creditsText;

    SoundIndex m_clickSound;
    SoundIndex m_selectSound;
};

