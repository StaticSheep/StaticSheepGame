if not menu then
  menu = {}
  menu.list = {}
  menu.meta = {}
end

function menu.Create(vStep, hStep, gamepads)
  -- Initialize button table
  print("Created menu")

  local newMenu = {}
  newMenu.buttons = {}

  newMenu.numButtons = 0
  newMenu.selected = 0
  newMenu.verticalStep = vStep or 1
  newMenu.horizontalStep = hStep or 1
  newMenu.validGamePads = gamepads or {}

  table.merge(menu.meta, newMenu)

  table.insert(menu.list, newMenu)

  newMenu.menuID = #menu.list

  return newMenu
end

function menu.Update()
  for id, realMenu in pairs(menu.list) do
    realMenu:Update()
  end
end

function menu.Delete(realMenu)
  local id = realMenu.menuID
  
  print("Deleted menu: "..id)

  menu.list[id]:Delete()

  table.remove(menu.list, id)
end




function menu.meta:SetHStep(step)
  self.horizontalStep = step
end

function menu.meta:SetVStep(step)
  self.verticalStep = step
end

function menu.meta:Delete()
  self.buttons = {}
end

function menu.meta:RegisterButton(btn)
  -- Insert button into menu
  table.insert(self.buttons, btn)

  self.numButtons = self.numButtons + 1

  -- Tell our button about its menu index
  btn.menuBtnID = self.numButtons
  btn.menuID = self.menuID

  -- If that was the first button to be added set it to hovered
  if self.numButtons == 1
    self.buttons[1]:SetHovered(true)
  end

  print("Registered button into menu at index: "..#menu.buttons)
end

function menu.meta:InsertButton(btn, index)
  -- Insert our button at the index in the menu
  table.insert(self.buttons, index, btn)

  btn.menuIndex = index

  self.numButtons = self.numButtons + 1

  -- It happened that we inserted a button in the place or before
  -- the currently selected button, we should update that
  if self.selected >= index then
    self.selected = self.selected + 1
  end

  print("Registered button into menu at index: "..index)
end

function menu.meta:RemoveButton(btn, index)
  -- Take out the removed button
  table.remove(self.button, index)

  self.numButtons = self.numButtons - 1

  if self.selected == index then
    self.selected = self.selected - 1
    self.button[self.selected]:SetHovered(true)
  elseif self.selected > index then
    self.selected = self.selected - 1
  end

  print("Removed button from menu at index: "..index)
end

function menu.meta:FindButtonIndex(btn)
  for index, button in pairs(self.buttons) do
    if button == btn then
      return index
    end
  end

  return -1
end

function menu.meta:Update()

  local clicked = false
  local hMove = 0
  local vMove = 0

  local newSelection = self.selected

  if hMove ~= 0 then
    hMove = hMove * self.horizontalStep
    newSelection = newSelection + hMove

    -- Wrap around if we went under 1 or above max buttons
    if newSelection < 1 then
      newSelection = self.numButtons + newSelection
    end

    if newSelection > self.numButtons then
      newSelection = newSelection - self.numButtons
    end

    -- Tell our old selection to no longer be selected
    self.buttons[self.selected]:SetHovered(false)

    -- Update our current selection
    self.selected = newSelection
    self.buttons[self.selected]:SetHovered(true)
  end

  if vMove ~= 0 then
    vMove = vMove * self.verticalStep
    newSelection = newSelection + vMove

    -- Wrap around if we went under 1 or above max buttons
    if newSelection < 1 then
      newSelection = self.numButtons + newSelection
    end

    if newSelection > self.numButtons then
      newSelection = newSelection - self.numButtons
    end

    -- Tell our old selection to no longer be selected
    self.buttons[self.selected]:SetHovered(false)

    -- Update our current selection
    self.selected = newSelection
    self.buttons[self.selected]:SetHovered(true)
  end

  if clicked then
    self.buttons[self.selected]:Press()
  end

end