if not menu then
  menu = {}
  menu.buttons = {}
  menu.selected = 0
  menu.numButtons = 0
  
  menu.verticalStep = 1
  menu.horizontalStep = 1
end



function menu.Create(vStep, hStep)
  -- Initialize button table
  print("Created menu")
  menu.buttons = {}

  menu.numButtons = 0
  menu.selected = 0
  menu.verticalStep = vStep or 1
  menu.horizontalStep = hStep or 1
end

function menu.Delete()
  print("Deleted menu")
  menu.buttons = {}
end

function menu.RegisterButton(btn)
  -- Insert button into menu
  table.insert(menu.buttons, btn)

  menu.numButtons = menu.numButtons + 1

  -- Tell our button about its menu index
  btn.menuIndex = menu.numButtons

  -- If that was the first button to be added set it to hovered
  if menu.numButtons == 1
    menu.buttons[1]:SetHovered(true)
  end

  print("Registered button into menu at index: "..#menu.buttons)
end

function menu.InsertButton(btn, index)
  -- Insert our button at the index in the menu
  table.insert(menu.buttons, index, btn)

  btn.menuIndex = index

  menu.numButtons = menu.numButtons + 1

  -- It happened that we inserted a button in the place or before
  -- the currently selected button, we should update that
  if menu.selected >= index then
    menu.selected = menu.selected + 1
  end

  print("Registered button into menu at index: "..index)
end

function menu.RemoveButton(btn, index)
  -- Take out the removed button
  table.remove(menu.button, index)

  menu.numButtons = menu.numButtons - 1

  if menu.selected == index then
    menu.selected = menu.selected - 1
    menu.button[menu.selected]:SetHovered(true)
  elseif menu.selected > index then
    menu.selected = menu.selected - 1
  end

  print("Removed button from menu at index: "..index)
end

function menu.FindButtonIndex(btn)
  for index, button in pairs(menu.buttons) do
    if button == btn then
      return index
    end
  end

  return -1
end

function menu.Update()

  local clicked = false
  local hMove = 0
  local vMove = 0

  local newSelection = menu.selected

  if hMove ~= 0 then
    hMove = hMove * menu.horizontalStep
    newSelection = newSelection + hMove

    -- Wrap around if we went under 1 or above max buttons
    if newSelection < 1 then
      newSelection = menu.numButtons + newSelection
    end

    if newSelection > menu.numButtons then
      newSelection = newSelection - menu.numButtons
    end

    -- Tell our old selection to no longer be selected
    menu.buttons[menu.selected]:SetHovered(false)

    -- Update our current selection
    menu.selected = newSelection
    menu.buttons[menu.selected]:SetHovered(true)
  end

  if vMove ~= 0 then
    vMove = vMove * menu.verticalStep
    newSelection = newSelection + vMove

    -- Wrap around if we went under 1 or above max buttons
    if newSelection < 1 then
      newSelection = menu.numButtons + newSelection
    end

    if newSelection > menu.numButtons then
      newSelection = newSelection - menu.numButtons
    end

    -- Tell our old selection to no longer be selected
    menu.buttons[menu.selected]:SetHovered(false)

    -- Update our current selection
    menu.selected = newSelection
    menu.buttons[menu.selected]:SetHovered(true)
  end

  if clicked then
    menu.buttons[menu.selected]:Press()
  end

end