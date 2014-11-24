/*****************************************************************
Filename: SheepFMOD.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef SHEEP_FMOD_H
#define SHEEP_FMOD_H

#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio.hpp"
#include "fmod_studio_common.h"
#include <string>

namespace SOUND = FMOD::Studio;

// Checks for FMOD errors
bool ErrorCheck(FMOD_RESULT result);

// callback function for sound files
FMOD_RESULT F_CALLBACK mycallback(FMOD_CHANNELCONTROL *chanControl, FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void *commandData1, void *commandData2);

// callback function for studio events
FMOD_RESULT F_CALLBACK mycallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters);

#endif // SHEEP_FMOD_H
