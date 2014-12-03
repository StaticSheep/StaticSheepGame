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