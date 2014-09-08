#include <iostream>
#include <Windows.h>
#include "SoundUtilities.h"
#include "SheepSoundManager.h"


using namespace SoundUtility;

namespace SOUND = FMOD::Studio;

void main(int argc, char **argv)
{
    int i = 0;

    std::cout << "Start of testing" << std::endl;

    SoundManager soundManager("GUIDs.txt");

    std::cout << "Start of testing" << std::endl;

    while(1)
    {
        if (soundManager.GetLoadState() && !i)
        {
            printf("DONE LOADING ALL BANKS AND EVENTS - PLAYING OUR FIRST SOUND");
            soundManager.Play(SFX_JAGUAR, PLAY_LOOP);
            ++i;
        }
        soundManager.Update();
    }

    system("pause");
}