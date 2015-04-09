--[[*****************************************************************
Filename: mainMenu.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("MainMenu")

InheritMeta(META, "LuaComponent")

function META:Register(pnl)
  table.insert(self.panels, pnl)
end

local function lerp(start, finish, t)
  return start + t*(finish - start)
end

local function Timed(time)
  return function(act)
  --print("BLAH")
  act.ttl = time
  act.tl = act.ttl
  end
end

local function Hold()
  return function(act, dt)
  act.tl = act.tl - dt
  if act.tl < 0 then
    act:Done()
  end
  end
end

local function FadeIn(self)
  return function(act, dt)
    self._alpha = lerp(0, 255, (1-(act.tl/act.ttl)))
    Hold()(act, dt)
  end
end

local function MoveButtonsX(self, tbl, start, finish)
  return function(act, dt)

    Hold()(act, dt)
    --print("A")
    local cur
    local t = (act.tl / act.ttl)
    if t > 1 then t = 1 end
    if t < 0 then t = 0 end
    cur = lerp(start, finish, 1 - t)
    for k,v in pairs(tbl) do
      v:SetPos(cur, v:GetPos().y)
    end

    
  end
end

function META:AddButton(text, menu, tbl, img_norm, img_hover, img_click)
  local btn = gui.Create("Button", self.base)
  self:Register(btn)
  menu:Add(btn)

  btn:SetText(text)
  btn:SetImage(img_norm, img_hover, img_click)
  btn:SetFont("aircruiser")

  btn:SetFontSize(14)
  btn:SetDSize(80, 16)

  btn:SetFontColor(Color(255, 255, 255, 255))
  btn:SetColor(Color(20, 20, 20, 0))
  btn:SetBGColor(Color(20, 20, 20, 0))

  btn:SetHoverColor(Color(100, 100, 100, 0))
  btn:SetHoverBGColor(Color(30, 30, 30, 0))
  btn:SetHoverFontColor(Color(200, 200, 120, 255))

  btn:SetClickColor(Color(100, 100, 100, 0))
  btn:SetClickBGColor(Color(20, 20, 20, 0))
  btn:SetClickFontColor(Color(255, 255, 120, 255))

  btn:SetPos(self.base:GetSize().x / 2 - btn:GetSize().x / 2,
    self.ypos)

  self.ypos = self.ypos + 50

  if tbl then
    table.insert(tbl, btn)
  end

  return btn
end

function META:MakeMenu()
  self.ypos = 0

  self.menu = CreateMenu(1, 0, {0, 1, 2, 3})

  self.base = gui.Create("FlatPane")
  self:Register(self.base)
  self.base:SetColor(Color(0, 0, 0, 0))
  self.base:SetDSize(640, 640)
  self.base:SetPos(0, 0, -1)
  self.base:SetCamMode(2)

  local title = gui.Create("Image", self.base)
  self:Register(title)
  
  title:SetTexture("GigaGravityTitle.png")
  title:SetDSize(Vec2(600, 150))
  title:SetPos(self.base:GetSize().x / 2 - title:GetSize().x / 2, 50)
  title:SetColor(Color(255, 255, 255))

  self.title = title

  self.ypos = title:GetSize().y + VWH / 7

  self.menuOffset = ScreenScale(500)
  print(self.menuOffset)

  local btn
  btn = self:AddButton("Start Game", self.menu, self.menuButtons)
  btn:SetOnPressed(function()

    self.menu:SetActive(false)

    self.List:PushBack(Action(
    Timed(0.5),
    FadeIn(self),
    nil,
    true))

    self.List:PushBack(Action(
    function(act)
      engine.ChangeLevel("Lobby")
    end,
    FadeIn(self),
    nil,
    true))


  end)

  btn = self:AddButton("Options", self.menu, self.menuButtons)
  btn:SetOnPressed(function()

    self:MakeOptionsMenu()


    self.List:PushBack(Action(
    function(act)
      Timed(0.25)(act)
    end,
    MoveButtonsX(self, self.menuButtons,
      self.menuButtons[1]:GetPos().x,
      self.menuButtons[1]:GetPos().x - self.menuOffset),
    function(act)
      
    end,
    false))

    self.List:PushBack(Action(
    function(act)
      --self:MakeOptionsMenu()
      Timed(0.25)(act)
    end,
    MoveButtonsX(self, self.optionButtons,
      self.optionButtons[1]:GetPos().x,
      self.optionButtons[1]:GetPos().x - self.menuOffset),
    function(act)
      self.optionMenu:SetActive(true)
      self.List:Clear()
    end,
    true))


    self.menu:SetActive(false)

    
    --self:CloseMenu()
  end)

  btn = self:AddButton("Credits", self.menu, self.menuButtons)
  btn:SetOnPressed(function()
    engine.ChangeLevel("Credits")

    --self:CloseMenu()
  end)

  -- btn = self:AddButton("How to Play")
  -- btn:SetOnPressed(function()
  --   self:MakeHelpMenu()
  -- end)

  btn = self:AddButton("Quit Game", self.menu, self.menuButtons)
  btn.firstPress = true
  btn:SetOnPressed(function()
    if btn.firstPress then
      btn.firstPress = false
      btn:SetText("Quit Game [CONFIRM]")
    else
      engine.Quit()
    end
  end)

  local rights = gui.Create("Label", self.base)
  self:Register(rights)
  rights:SetPos(640 / 2, 620)
  rights:SetDSize(5.5)
  rights:SetText("All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.\nFMOD Studio Copyright (c) 2005-2011 Firelight Technologies Pty, Ltd.")
  rights:SetXAlignment(TEXT_ALIGN_CENTER)
  rights:SetYAlignment(TEXT_ALIGN_BOTTOM)
end

function round(v, p)
local mult = math.pow(10, p or 0) -- round to 0 places when p not supplied
    return math.floor(v * mult + 0.5) / mult
end

local function UpdateMusicButton(btn, vol)

  if vol then
    engine.SetMusicVolume(vol)
  end

  local level = round(engine.GetMusicVolume(), 1)
  btn.level = level
  level = level * 100

  btn:SetText("Music: "..level.."%")
end

local function UpdateSFXButton(btn, vol)

  if vol then
    engine.SetSFXVolume(vol)
  end

  local level = round(engine.GetSFXVolume(), 1)
  btn.level = level
  level = level * 100

  btn:SetText("SFX: "..level.."%")
end

local function UpdateMasterButton(btn, vol)

  if vol then
    engine.SetMasterVolume(vol)
  end

  local level = round(engine.GetMasterVolume(), 1)
  btn.level = level
  level = level * 100

  btn:SetText("Master: "..level.."%")
end

function META:MakeOptionsMenu()
  self.optionMenu = CreateMenu(1, 0, {0, 1, 2, 3})
  self.optionMenu:SetActive(false)

  self.ypos = self.title:GetSize().y + VWH / 7

  local btn
  btn = self:AddButton("Master: On", self.optionMenu, self.optionButtons)
  UpdateMasterButton(btn)

  local pos = btn:GetPos()
  btn:SetPos(pos.x + self.menuOffset, pos.y)
  btn:SetOnPressed(function(bself)
    bself.level = bself.level + 0.1
    if bself.level > 1.0 then
      bself.level = 0.0
    end
    UpdateMasterButton(bself, bself.level)
  end)

  local btn
  btn = self:AddButton("Music: On", self.optionMenu, self.optionButtons)
  UpdateMusicButton(btn)

  local pos = btn:GetPos()
  btn:SetPos(pos.x + self.menuOffset, pos.y)
  btn:SetOnPressed(function(bself)
    bself.level = bself.level + 0.1
    if bself.level > 1.0 then
      bself.level = 0.0
    end
    UpdateMusicButton(bself, bself.level)
  end)

  local btn
  btn = self:AddButton("SFX: On", self.optionMenu, self.optionButtons)
  UpdateSFXButton(btn)

  local pos = btn:GetPos()
  btn:SetPos(pos.x + self.menuOffset, pos.y)
  btn:SetOnPressed(function(bself)
    bself.level = bself.level + 0.1
    if bself.level > 1.0 then
      bself.level = 0.0
    end
    UpdateSFXButton(bself, bself.level)
  end)

  btn = self:AddButton("Fullscreen", self.optionMenu, self.optionButtons)
  pos = btn:GetPos()
  btn:SetPos(pos.x + self.menuOffset, pos.y)
  btn:SetOnPressed(function()
    surface.SetFullScreen(not surface.GetFullScreen())
    --self:MakeOptionsMenu()
    --self.optionMenu:Select(2)
  end)

  -- btn = self:AddButton("Credits", self.optionMenu, self.optionButtons)
  -- btn:SetOnPressed(function()
  --   --self:CloseMenu()
  -- end)

  -- btn = self:AddButton("How to Play")
  -- btn:SetOnPressed(function()
  --   self:MakeHelpMenu()
  -- end)

  btn = self:AddButton("Return", self.optionMenu, self.optionButtons)
  pos = btn:GetPos()
  btn:SetPos(pos.x + self.menuOffset, pos.y)
  btn:SetOnPressed(function()

    self.optionMenu:SetActive(false)

    self.List:PushBack(Action(
    function(act)
      
      --self:MakeOptionsMenu()
      Timed(0.25)(act)
    end,
    MoveButtonsX(self, self.optionButtons,
      self.optionButtons[1]:GetPos().x,
      self.optionButtons[1]:GetPos().x + self.menuOffset),
    function(act)
      for k,v in pairs(self.optionButtons) do
        v:Destroy()
      end
      self.optionButtons = {}
      self.optionMenu:Destroy()
      self.optionMenu = nil
      --self.optionMenu:SetActive(true)
    end,
    false))


    self.List:PushBack(Action(
    function(act)
      Timed(0.25)(act)
    end,
    MoveButtonsX(self, self.menuButtons,
      self.menuButtons[1]:GetPos().x,
      self.menuButtons[1]:GetPos().x + self.menuOffset),
    function(act)
      if self.menu then
        self.menu:SetActive(true)
      end

      self.List:Clear()
    end,
    true))

  end)

end


function META:CloseMenu()
  --self:HideHelpMenu()
  self._opened = false
  self.menu:SetActive(false)
end

function META:CleanUp()
  for k,v in pairs(self.panels) do
    v:Destroy()
  end

  self.panels = {}

  if self.menu ~= nil then
    self.menu:Destroy()
    self.menu = nil
  end

  if self.optionMenu ~= nil then
    self.optionMenu:Destroy()
    self.optionMenu = nil
  end

  if self.creditsmenu ~= nil then
    self.creditsmenu:Destroy()
    self.creditsmenu = nil
  end

  self.menuButtons = {}
  self.optionButtons = {}
  self.creditButtons = {}

  self.base = nil
  self._alpha = 0

  if self.List then
    if self.List.Clear then
      self.List:Clear()
    end
  end

end

function META:Init()
  self:Owner().SoundPlayer:PlayLoop("Main Music")

  self.panels = {}
  self.menu = nil
  self.optionMenu = nil
  self.creditsmenu = nil
  self._alpha = 0

  self.List = actionlist.Create()

  self.menuButtons = {}
  self.optionButtons = {}

  self.super.Init(self)

  -- self:PauseSpace(true)


  self:MakeMenu()
  -- self:MakeHelpMenu()

  --self.base.pos.x = 0
end

function META:Refresh()
  --self:PauseSpace(true)
  --self:CleanUp()
  
  self:CleanUp()
  self:MakeMenu()
  --self:MakeHelpMenu()

  --self:PauseSpace(true)
  --self:MakeMenu()
  --self:MakeMenu()
end

function META:Remove()
  self:CleanUp()
  hook.Remove("FrameUpdate", self)
  hook.Remove("LogicUpdate", self)
  hook.Remove("PostDraw", self)
  --hook.Remove("ScreenResize", self)
end

function META:Draw()
  if self._alpha > 0 then
    surface.SetColor(0, 0, 0, self._alpha)
    surface.ForceZ(true, -1.0)
    surface.DrawRect(0, 0, 1920, 1080)
    surface.ForceZ(false, 0)
  end
end

function META:SetupHooks()
  hook.Add("FrameUpdate", self, self.FrameUpdate)
  hook.Add("LogicUpdate", self, self.LogicUpdate, self.CanUpdate)
  hook.Add("PostDraw", self, self.Draw)
  --hook.Add("ScreenResize", self, self.Refresh)
end

function META:PauseSpace(paused)
  audio.PauseAll(paused)
  gamespace.Get(self._space)._pointer:SetPaused(paused)
end

function META:LogicUpdate(dt)
  
end

function META:FrameUpdate(dt)
  self.List:Update(dt)
end

RegisterComponent(META)