/*****************************************************************
Filename: AntTweak.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "AntTweak\AntTweakBar.h"
#include "AntTweakModule.h"

#include "Window.h"

#include "SheepGraphics.h"

namespace Framework
{
  AntTweakModule* ATWEAK = nullptr;

  AntTweakModule::AntTweakModule()
    :m_bars(sizeof(TBar), 10)
  {
    ATWEAK = this;
  }

  void AntTweakModule::Initialize()
  {
    TBar* bc = CreateBar("TestBar");
    float* f = new float(10);
    bc->AddVarRW("SomeFloat", AntTweak::TW_TYPE_FLOAT, f);
  }

  void AntTweakModule::ReceiveMessage(Message msg)
  {
#if USE_ANTTWEAKBAR
    if (msg.MessageId == Message::PostDraw)
    {
      TwDraw();
      return;
    }
    if (msg.MessageId == Message::GFXDeviceInit)
    {
      TwInit(TW_DIRECT3D11, GRAPHICS->GetDevice());
      TwWindowSize(ENGINE->Window->GetWidth(), ENGINE->Window->GetHeight());
      return;
    }
#endif
  }

  AntTweakModule::TBar* AntTweakModule::CreateBar(const char* name)
  {
#if USE_ANTTWEAKBAR
    // Allocates space for a new bar container
    TBar* newBar = (TBar*)m_bars.Allocate();

    // Placement new on our new container
    new (newBar) TBar(name);

    // Creates a new AntTweakBar and sets the pointer
    newBar->antTweakBar = TwNewBar(name);
    // Registers the container to the handle manager
    newBar->self = m_handles.Insert(newBar);


    // Sync any handles needed
    m_handles.SyncHandles<TBar>(m_bars);

    return newBar;
#else
    return nullptr;
#endif
  }

  AntTweakModule::TBar* AntTweakModule::GetBar(Handle barHandle)
  {
#if USE_ANTTWEAKBAR
    // Gets the container from the handle manager
    return m_handles.GetAs<TBar>(barHandle);
#else
    return nullptr;
#endif
  }

  void AntTweakModule::Update(float dt)
  {
#if USE_ANTTWEAKBAR

#endif
  }

  void AntTweakModule::Shutdown()
  {
#if USE_ANTTWEAKBAR
    TwTerminate();
#endif
  }




  // Adds a definition to use when creating buttons/vars
  void AntTweakModule::TBar::AddDefinition(const char* definition, bool persist)
  {
#if USE_ANTTWEAKBAR
    if (persist)
      m_pDefinitions.push_back(definition);
    else
      m_definitions.push_back(definition);
#endif
  }
  // Clears the vector of persistent definitions
  void AntTweakModule::TBar::ClearDefinitions(void)
  {
#if USE_ANTTWEAKBAR
    m_pDefinitions.clear();
#endif
  }

  // Sets the help message for the next button/var
  void AntTweakModule::TBar::DefineHelpMessage(const char* message)
  {
#if USE_ANTTWEAKBAR
    std::string helpMessage(" help='");
    helpMessage += message;
    helpMessage += "' ";
    // help='<message>' 

    m_definitions.push_back(helpMessage);
#endif
  }
  // Sets the group for the next button/var
  void AntTweakModule::TBar::DefineGroup(const char* group)
  {
#if USE_ANTTWEAKBAR
    std::string groupName(" group=");
    groupName += group;
    groupName += " ";
    // group=<group> 

    m_definitions.push_back(groupName);
#endif
  }
  // Sets if the next button/var is visible
  void AntTweakModule::TBar::DefineVisible(bool visible)
  {
#if USE_ANTTWEAKBAR
    std::string def(" visible=");
    def += visible ? "true" : "false";
    def += " ";

    m_definitions.push_back(def);
#endif
  }
  // Sets a min/max for the next var
  // If the next value is an integer, then it will truncate
  void AntTweakModule::TBar::DefineMinMax(float min, float max)
  {
#if USE_ANTTWEAKBAR
    std::string def(" min=");
    def += std::to_string(min);
    def += " max=";
    def += std::to_string(max);
    def += " ";

    m_definitions.push_back(def);
#endif
  }
  // Sets a step value for the variable if it is a slider
  void AntTweakModule::TBar::DefineStep(float step)
  {
#if USE_ANTTWEAKBAR
    std::string def(" step=");
    def += std::to_string(step);
    def += " ";

    m_definitions.push_back(def);
#endif
  }
  // Sets a keyboard shortcut for a button
  // Refer to http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:varparamsyntax for help
  void AntTweakModule::TBar::DefineKeyShortcut(const char* key)
  {
#if USE_ANTTWEAKBAR
    std::string def(" key=");
    def += key;
    def += " ";

    m_definitions.push_back(def);
#endif
  }
  // Sets a keyboard shortcut for Increment and Decrement
  void AntTweakModule::TBar::DefineIncDecShortcut(const char* inc, const char* dec)
  {
#if USE_ANTTWEAKBAR
    std::string def(" keyincr=");
    def += inc;
    def += " keydecr=";
    def += dec;
    def += " ";

    m_definitions.push_back(def);
#endif
  }
  // Sets a boolean value for the next var
  void AntTweakModule::TBar::DefineBool(const char* isTrue, const char* isFalse)
  {
#if USE_ANTTWEAKBAR
    std::string def(" true='");
    def += isTrue;
    def += "' false='";
    def += isFalse;
    def += "' ";

    m_definitions.push_back(def);
#endif
  }

  // Opens or closes a variable/button grouping in a bar
  void AntTweakModule::TBar::SetGroupOpened(const char* groupName, bool isOpen)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, groupName, "opened", TW_PARAM_INT32, 1, &isOpen);
#endif
  }




  void AntTweakModule::TBar::AddVarCB(const char* name, AntTweak::TwType type, AntTweakModule::aTSetCB setCB, AntTweakModule::aTGetCB getCB, void* clientData)
  {
#if USE_ANTTWEAKBAR
    std::string defList;
    for (size_t i=0; i < m_definitions.size(); ++i)
    {
      defList += m_definitions[i];
    }
    for (size_t i=0; i < m_pDefinitions.size(); ++i)
    {
      defList += m_definitions[i];
    }

    TwType realType = (TwType)TranslateType(type);

    TwAddVarCB((TwBar*)antTweakBar, name, realType, setCB, getCB, clientData, defList.c_str());
#endif
  }

  void AntTweakModule::TBar::AddVarRW(const char* name, AntTweak::TwType type, void* var)
  {
#if USE_ANTTWEAKBAR
    std::string defList;
    for (size_t i=0; i < m_definitions.size(); ++i)
    {
      defList += m_definitions[i];
    }
    for (size_t i=0; i < m_pDefinitions.size(); ++i)
    {
      defList += m_definitions[i];
    }

    TwType realType = (TwType)TranslateType(type);

    TwAddVarRW((TwBar*)antTweakBar, name, realType, var, defList.c_str());
#endif
  }

  void AntTweakModule::TBar::AddSeparator(const char* name)
  {
#if USE_ANTTWEAKBAR
    std::string defList;
    for (size_t i=0; i < m_definitions.size(); ++i)
    {
      defList += m_definitions[i];
    }
    for (size_t i=0; i < m_pDefinitions.size(); ++i)
    {
      defList += m_definitions[i];
    }

    TwAddSeparator((TwBar*)antTweakBar, name, defList.c_str());
#endif
  }


  AntTweak::TwType AntTweakModule::TBar::TranslateType(AntTweak::TwType type)
  {
    // @TODO
    return type;
  }





}