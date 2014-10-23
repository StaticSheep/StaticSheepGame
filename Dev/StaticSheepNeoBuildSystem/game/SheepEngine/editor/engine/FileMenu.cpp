
#include "pch/precompiled.h"

#include "editor/App.h"

#include "engine/core/Engine.h"

namespace dit 
{

  using namespace Framework;

  void EditorWindow::OnNewSpace(wxCommandEvent& event)
  {
    ENGINE->CreateSpace(nullptr);
  }

}