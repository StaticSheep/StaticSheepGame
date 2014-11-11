

local PanelFactory = {}

local panelMetaTable = GetMeta("Panel")

if gui == nil then gui = {} end
if gui.panels == nil then gui.panels = {} end
if gui.panelcounter == nil then gui.panelcounter = 0 end

local table = {}
function table.Merge(dest, source)

  for k,v in pairs(source) do
  
    if ( type(v) == 'table' and type(dest[k]) == 'table' ) then
      -- don't overwrite one table with another;
      -- instead merge them recurisvely
      table.Merge(dest[k], v)
    else
      dest[k] = v
    end
  end
  
  return dest
end

function gui.Inherit( meta, base )
  meta.BasePanel = base
end

function gui.Create( classname, parent, name, ...)
  local panel = gui.Make(classname, parent, name, ...)
  panel.id = gui.panelcounter

  gui.panels[gui.panelcounter] = panel

  gui.panelcounter = gui.panelcounter + 1

  return panel
end

function gui.Make( classname, parent, name, ...)
  if PanelFactory[classname] ~= nil then

    local metatable = PanelFactory[classname]
    local panel = gui.Make( metatable.BasePanel, parent, name )
    if ( not panel ) then
      Error( "Tried to create panel with invalid base '"..metatable.BasePanel.."'\n" );
    end

    table.Merge( panel, metatable )
    panel.BaseClass = PanelFactory[ metatable.BasePanel ]
    panel.ClassName = classname
    
    -- Call the Init function if we have it
    if ( panel.Init ) then
      panel:Init()  
    end
      
    panel:Prepare()
    
    return panel
  else
    -- Must be the base Panel class
    local panel = setmetatable({}, panelMetaTable)

    if panel.Init then
      panel:Init()
    end

    panel:Prepare()

    return panel
  end
end

function gui.Register( name, mtable, base )

  -- Remove the global
  PANEL = nil

  -- Default base is Panel
  mtable.BasePanel = base or "Panel"
  
  PanelFactory[ name ] = mtable

  -- local mt = {}
  -- mt.__index = function( t, k )
  
  --   if ( PanelFactory[ mtable.BasePanel ] && PanelFactory[ mtable.BasePanel ][k] ) then return PanelFactory[ mtable.BasePanel ][k] end
  --   return panel_metatable[k]

  -- end

  -- setmetatable( mtable, mt )

  return mtable

end

function gui.Update()
  local removeList = {}
  for k,v in pairs(gui.panels) do
    v:Think()
    if not v.valid then
      removeList[#removeList] = v
    end
  end
  for k,v in pairs(removeList) do
    v:Delete()
    gui.panels[v.id] = nil
  end
end

function gui.Draw()
  for k,v in pairs(gui.panels) do
    if v.visible then
      v:Paint()
    end
  end
end