/*****************************************************************
Filename: SheepFMOD.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

////////////////////////////////////////////////////////////////////////////////
#ifndef SHEEP_FMOD_H
#define SHEEP_FMOD_H
////////////////////////////////////////////////////////////////////////////////

#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"
#include "FMOD/fmod_studio.hpp"
#include "FMOD/fmod_studio_common.h"
#include <string>

namespace SOUND = FMOD::Studio;

void ErrorCheck(FMOD_RESULT result);

////////////////////////////////////////////////////////////////////////////////
#endif // SHEEP_FMOD_H
////////////////////////////////////////////////////////////////////////////////
