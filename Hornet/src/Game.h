#pragma once
#include <vector>
#include "../Engine/HtGraphics.h"
#include "../Engine/HtKeyboard.h"
#include "../Engine/HtMouse.h"
#include "../Engine/HtAudio.h"
#include "../Engine/GameTimer.h"

// 17/06/2023
// This class is where you will start programming a game.
// The core of the game loop is Update(), which is called from HornetApp.
//
// 6/12/2023
// Added engine #includes to the header files. We are going to need them anyway.

class Game
{
public:
    // Use this function for any code when the program starts.
    // This could be empty
    void StartOfProgram();

    // Use this function to set up your game each time it starts
    void StartOfGame();

    // This function runs one "frame of animation".
    // Actually, in a windows environment it will probably
    // run much more often than that.
    // double frametime - the time in seconds since the last frame
    void Update(double frameTime);

    // This function is for when the program goes to the pause menu
    // You may want to pause sound and music effects, or set the
    // background colour
    void OnSuspend();

    // This function is for when the program returns from the pause menu
    // You may want to resume sound and music effects, or set the
    // background colour
    void OnResume();

    // Use this function to delete any objects at the end of the game.
    // You may want to stop sound and music.
    void EndOfGame();

    // Use this function to delete any objects before the program exits.
    // This may well be empty
    void EndOfProgram();

private:
    GameTimer m_timer;
};

