/*****************************************************************
Filename: Qualifiers.h
Project: 
Author(s): Zachary Nawar (Primary)

Nifty library which I pretty much just copied from Randy Gual.
It will strip off any sort of qualifiers from a variable type
so that you are only left with the bare type.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  template <typename T>
  struct StripMemberness;

  template <typename T, typename U>
  struct StripMemberness<T U::*>
  {
    typedef T type;
  };

  template <typename T>
  struct RemoveQualifiers
  {
    typedef T type;
  };

  template <typename T>
  struct RemoveQualifiers<const T>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<T&>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<const T&>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<T&&>
  {
    typedef typename RemoveQualifiers<T>::type type;
  };

  template <typename T>
  struct RemoveQualifiers<const T*>
  {
    typedef typename RemoveQualifiers<T*>::type type;
  };

  template <typename T>
  struct RemovePtr
  {
    typedef T type;
  };

  template <typename T>
  struct RemovePtr<T*>
  {
    typedef typename RemovePtr<T>::type type;
  };
}