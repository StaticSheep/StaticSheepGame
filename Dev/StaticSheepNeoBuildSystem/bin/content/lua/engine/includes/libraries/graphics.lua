--[[*****************************************************************
Filename: graphics.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]

SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

function WindowResize(width, height)
  SCREEN_WIDTH = width
  SCREEN_HEIGHT = height
  hook.Call("ScreenResize", width, height)
end

function ScrW()
  return SCREEN_WIDTH
end

function ScrH()
  return SCREEN_HEIGHT
end

function CheckDimensions()
  SCREEN_WIDTH = GetScrW()
  SCREEN_HEIGHT = GetScrH()
end

VWW = 640.0
VWH = 640.0

function ScreenScale(size)
  return size * (ScrW() / VWW)
end

function ScreenScaleY(size)
  return size * (ScrH() / VWH)
end