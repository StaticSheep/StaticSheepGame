/*****************************************************************
Filename: SafeRelease.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

template <typename T1>
void SafeRelease(T1 toRelease)
{
  if (toRelease)
  {
    toRelease->Release();
    toRelease = NULL;
  }
}