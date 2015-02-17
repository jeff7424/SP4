#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <irrKlang.h>
#include "AudioPlayer.h"

using namespace irrklang;
using namespace std;
 
/*******************************
		  CONSTRUCTORS
*******************************/

AudioPlayer::AudioPlayer()
{
    se = createIrrKlangDevice();
    currentSound = 0;
    if(!se)
    {
        cout << "Error: Could not create sound engine" << endl;
    } 
	fileName = "\0";
	volume = 50;
	position = 0;
	se->setSoundVolume(1);
}

AudioPlayer::AudioPlayer(string soundFile)
{
    se = createIrrKlangDevice();
    currentSound = 0;
    if(!se)
    {
        cout << "Error: Could not create sound engine" << endl;
        exit(0);
    }

    fileName = soundFile;
    volume = 50;
    position = 0;
    se->setSoundVolume(1);
}

/*****************************
		 DESTRUCTOR
*****************************/

AudioPlayer::~AudioPlayer()
{
}

/***************************
	   PLAY FUNCTIONS
***************************/

void AudioPlayer::playSound()
{
    if(fileName == "\0")
    {
        cout << "Error: No sound file selected" << endl;
        return;
    }
    currentSound = se->play2D(fileName.c_str(),true, false, true);

    if(!currentSound)
    {
        cout << "Error: Could not play file" << endl;
    }
    while(!currentSound->isFinished())
    Sleep(100);
    position = currentSound->getPlayPosition();
}

void AudioPlayer::playSound(int milliseconds)
{
    if(fileName == "\0")
    {
        cout << "Error: No sound file selected" << endl;
        return;
	}
    currentSound = se->play2D(fileName.c_str(),true, false, true);
    if(!currentSound)
    {
        cout << "Error: Could not play file" << endl;
        exit(0);
    }
    Sleep(milliseconds);
    pause();
}

void AudioPlayer::playSound(string soundFile)
{
    setFileName(soundFile);
    currentSound = se->play2D(fileName.c_str(),false, false, true);

    if(!currentSound)
    {
        cout << "Error: Could not play file" << endl;
        exit(0);
    }

    while(!currentSound->isFinished())
	{
        Sleep(100);
	}
    position = currentSound->getPlayPosition();
}

void AudioPlayer::playSound(string soundFile, int milliseconds)
{
    setFileName(soundFile);
    currentSound = se->play2D(fileName.c_str(),true, false, true);
    if(!currentSound)
    {
        cout << "Error: Could not play file" << endl;
        exit(0);
    }
    Sleep(milliseconds);
    pause();
}

void AudioPlayer::playSoundThreaded()
{
    currentSound = se->play2D(fileName.c_str(),true, false, true);
    if(!currentSound)
    {
        cout << "Error: Could not play file" << endl;
        exit(0);
    }
}

bool AudioPlayer::isSoundPlaying()
{
    return !currentSound->isFinished();
}

/************************
	PAUSE/RESUME/STOP
************************/

void AudioPlayer::pause()
{
    position = currentSound->getPlayPosition();
    se->setAllSoundsPaused();
}

void AudioPlayer::resume()
{
    currentSound = se->play2D(fileName.c_str(), true, false, true);   
	
	if(position != -1)
	{
		currentSound->setPlayPosition(position);
	}
}

void AudioPlayer::stop()
{
    se->stopAllSounds();
}

/************************
	GET/SET FILENAME
************************/

void AudioPlayer::setFileName(string soundFile)
{
    if(soundFile != "")
	{
        fileName = soundFile;
	}
}

string AudioPlayer::getFileName()
{
    return fileName;
}

/***************************
	   VOLUME CONTROL
***************************/

void AudioPlayer::setVolume(int newVolume)
{
    if(newVolume < 0 || newVolume > 100)
	{
        volume = 50;
	}
	else
	{
        volume = newVolume;
	}
    currentSound->setVolume(volume/100.0);
}

void AudioPlayer::increaseVolume()
{
    volume += 5; 
    if(volume > 100)
	{
        volume = 100;
	}
	currentSound->setVolume(volume/100.0);
}

void AudioPlayer::increaseVolume(int increment)
{  
    volume += increment;
    if(volume > 100)
	{
        volume = 100;
	}
	currentSound->setVolume(volume/100.0);
}

void AudioPlayer::decreaseVolume()
{
    volume -= 5;
    if(volume < 0)
	{
        volume = 0;
	}
	currentSound->setVolume(volume/100.0);
}

void AudioPlayer::decreaseVolume(int increment)
{
    volume -= increment;

    if(volume < 0)
	{
		volume = 0;
	}
	currentSound->setVolume(volume/100.0);
}

int AudioPlayer::getCurrentVolume()
{
    return volume;
}
