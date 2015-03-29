--[[*****************************************************************
Filename: actionlist.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
if not actionlist then
  actionlist = {}
  actionlist.listMeta = {}
  actionlist.actionMeta = {}
end

--[[ Action List Meta class ]]

function actionlist.listMeta:Init()
  self.actions = {}
end

function actionlist.listMeta:Clear()
  self.actions = {}
end

function actionlist.listMeta:Update(dt)

  for k,action in pairs(self.actions) do

    if not action.invalid then

      if not action.running then
        action:OnStart()
        action.running = true
      end


      action:Update(dt)
      if action:IsBlocking() then
        break
      end

      if action:IsFinished() then
        action:OnEnd()
        action.invalid = true
      end
    else

    end
  end

end

function actionlist.listMeta:PushBack(action)
  action.owner = self
  table.insert(self.actions, action)
end

function actionlist.listMeta:IsEmpty()
  return #self.actions == 0
end


--[[ Action Meta class ]]

function actionlist.actionMeta:Init()
  self.owner = nil
  self.running = false
  self.invalid = false
  self.isFinished = false
  self.isBlocking = false
end

function actionlist.actionMeta:Update(dt)

end

function actionlist.actionMeta:OnStart()
  print("START")
end

function actionlist.actionMeta:OnEnd()

end

function actionlist.actionMeta:Done()
  self.isFinished = true
  self.isBlocking = false
end

function actionlist.actionMeta:IsBlocking()
  return self.isBlocking
end

function actionlist.actionMeta:IsFinished()
  return self.isFinished
end

function actionlist.Create()
  local list = table.Merge({}, actionlist.listMeta)
  list:Init()
  return list
end

function Action(onStart, update, onEnd, blocking)
  local act = table.Merge({}, actionlist.actionMeta)
  act:Init()

  if (onStart) then
    act.OnStart = onStart
  end
  if (update) then
    act.Update = update
  end
  if (onEnd) then
    act.OnEnd = onEnd
  end
  act.isBlocking = blocking
  
  return act
end