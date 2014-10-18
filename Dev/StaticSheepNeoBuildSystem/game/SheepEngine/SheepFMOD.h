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

#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio.hpp"
#include "fmod_studio_common.h"
#include <string>

namespace SOUND = FMOD::Studio;

bool ErrorCheck(FMOD_RESULT result);

////////////////////////////////////////////////////////////////////////////////
#endif // SHEEP_FMOD_H
////////////////////////////////////////////////////////////////////////////////
