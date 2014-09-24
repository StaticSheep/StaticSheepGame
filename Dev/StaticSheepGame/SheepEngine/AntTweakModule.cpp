/*****************************************************************
Filename: AntTweak.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "AntTweak\AntTweakBar.h"
#include "AntTweakModule.h"
#include "TweakHelper.h"


#include "Window.h"

#include "SheepGraphics.h"

namespace Framework
{
  AntTweakModule* ATWEAK = nullptr;

  void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
  {
    destinationClientString = sourceLibraryString;
  }

  AntTweakModule::AntTweakModule()
    :m_bars(sizeof(AntTweak::TBar), 10)
  {
    ATWEAK = this;

  }

  AntTweakModule::~AntTweakModule()
  {
    ATWEAK = nullptr;
  }

  void AntTweakModule::Initialize()
  {
    
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
      TwCopyStdStringToClientFunc(CopyStdStringToClient); // must be called once (just after TwInit for instance)
      
      return;
    }
#endif
  }

  AntTweak::TBar* AntTweakModule::CreateBar(const char* name)
  {
#if USE_ANTTWEAKBAR
    // Allocates space for a new bar container
    AntTweak::TBar* newBar = (AntTweak::TBar*)m_bars.Allocate();

    // Placement new on our new container
    new (newBar) AntTweak::TBar(name);

    // Creates a new AntTweakBar and sets the pointer
    newBar->antTweakBar = TwNewBar(name);
    // Registers the container to the handle manager
    newBar->self = m_handles.Insert(newBar);


    // Sync any handles needed
    m_handles.SyncHandles<AntTweak::TBar>(m_bars);

    return newBar;
#else
    return nullptr;
#endif
  }

  void AntTweakModule::RemoveBar(AntTweak::TBar* bar)
  {
#if USE_ANTTWEAKBAR

    TwDeleteBar((TwBar*)bar->antTweakBar);
    bar->~TBar();

    AntTweak::TBar* moved = (AntTweak::TBar*)m_bars.Free(bar);
    if (moved)
      m_handles.Update(moved, moved->self);
    
#endif
  }

  AntTweak::TBar* AntTweakModule::GetBar(Handle barHandle)
  {
#if USE_ANTTWEAKBAR
    // Gets the container from the handle manager
    return m_handles.GetAs<AntTweak::TBar>(barHandle);
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
    for(size_t i = 0; i < m_bars.Size(); ++i)
    {
      ((AntTweak::TBar*)(m_bars[i]))->~TBar();
    }
#if USE_ANTTWEAKBAR
    TwDeleteAllBars();
    TwTerminate();
#endif
  }




  // Adds a definition to use when creating buttons/vars
  void AntTweak::TBar::AddDefinition(const char* definition, bool persist)
  {
#if USE_ANTTWEAKBAR
    if (persist)
      m_pDefinitions.push_back(definition);
    else
      m_definitions.push_back(definition);
#endif
  }
  // Clears the vector of persistent definitions
  void AntTweak::TBar::ClearDefinitions(void)
  {
#if USE_ANTTWEAKBAR
    m_pDefinitions.clear();
#endif
  }

  void AntTweak::TBar::DefineLabel(const char* label)
  {
#if USE_ANTTWEAKBAR
    std::string helpMessage(" label='");
    helpMessage += label;
    helpMessage += "' ";
    // help='<message>' 

    m_definitions.push_back(helpMessage);
#endif
  }

  // Sets the help message for the next button/var
  void AntTweak::TBar::DefineHelpMessage(const char* message)
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
  void AntTweak::TBar::DefineGroup(const char* group)
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
  void AntTweak::TBar::DefineVisible(bool visible)
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
  void AntTweak::TBar::DefineMinMax(float min, float max)
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
  void AntTweak::TBar::DefineStep(float step)
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
  void AntTweak::TBar::DefineKeyShortcut(const char* key)
  {
#if USE_ANTTWEAKBAR
    std::string def(" key=");
    def += key;
    def += " ";

    m_definitions.push_back(def);
#endif
  }
  // Sets a keyboard shortcut for Increment and Decrement
  void AntTweak::TBar::DefineIncDecShortcut(const char* inc, const char* dec)
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
  void AntTweak::TBar::DefineBool(const char* isTrue, const char* isFalse)
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
  void AntTweak::TBar::SetGroupOpened(const char* groupName, bool isOpen)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, groupName, "opened", TW_PARAM_INT32, 1, &isOpen);
#endif
  }


  // Sets a bar label
  void AntTweak::TBar::SetLabel(const char* name)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "label", TW_PARAM_CSTRING, 1, name);
#endif
  }

  // Defines the help message associated with the bar
  void AntTweak::TBar::SetHelpLabel(const char* name)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "help", TW_PARAM_CSTRING, 1, name);
#endif
  }

  // Set Bar Color
  void AntTweak::TBar::SetColor(int red, int green, int blue)
  {
#if USE_ANTTWEAKBAR
    int color[3] = {red, green, blue};
    TwSetParam((TwBar*)antTweakBar, NULL, "label", TW_PARAM_INT32, 3, &color);
#endif
  }

  // Sets the alpha-value of the bar
  void AntTweak::TBar::SetAlpha(int alpha)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "alpha", TW_PARAM_INT32, 1, &alpha);
#endif
  }

  // Sets the position of the bar
  void AntTweak::TBar::SetPos(int x, int y)
  {
#if USE_ANTTWEAKBAR
    int pos[2] = {x, y};
    TwSetParam((TwBar*)antTweakBar, NULL, "position", TW_PARAM_INT32, 2, &pos);
#endif
  }

  // Sets the size of the bar
  void AntTweak::TBar::SetSize(int x, int y)
  {
#if USE_ANTTWEAKBAR
    int pos[2] = {x, y};
    TwSetParam((TwBar*)antTweakBar, NULL, "size", TW_PARAM_INT32, 2, &pos);
#endif
  }

  // Sets the bar refresh rate
  void AntTweak::TBar::SetRefreshRate(float rate)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "refresh", TW_PARAM_FLOAT, 1, &rate);
#endif
  }

  // Sets whether the bar is iconified or not
  void AntTweak::TBar::SetIconify(bool icon)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "iconifiable", TW_PARAM_INT32, 1, &icon);
#endif
  }

  // Sets whether the bar is movable or not
  void AntTweak::TBar::SetMovable(bool canMove)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "movable", TW_PARAM_INT32, 1, &canMove);
#endif
  }

  // Sets whether the bar is re-sizable
  void AntTweak::TBar::SetResizable(bool canResize)
  {
#if USE_ANTTWEAKBAR
    TwSetParam((TwBar*)antTweakBar, NULL, "resizable", TW_PARAM_INT32, 1, &canResize);
#endif
  }

  /*-----------------------------------------------------------------------------------
  
  Generic Callbacks:
  Callback functions for setting/getting variables from generic objects

  ------------------------------------------------------------------------------------*/

  static void TW_CALL GenericSetCB(const void* value, void* rawData)
  {
    AntTweak::TweakGenericVar* clientData = (AntTweak::TweakGenericVar*)rawData;

    // Get the type of the member we are changing
    const TypeInfo* memberType = clientData->genericMember->Type();
    // Get a pointer to the generic object
    void* genericObject = clientData->genericSpace->GetHandles().Get(clientData->genericHandle);

    ErrorIf(genericObject == nullptr, "AntTweakBar GenericObject Variable Set", "Attempted to set a variable on an object which couldn't be found! Member: %s",
      clientData->genericMember->Name());

    void* data = (char*)genericObject + clientData->genericMember->Offset();

    if (clientData->setCB)
    {
      clientData->setCB(data, clientData);
    }
    else
    {
      // Use the types copy/assignment operation to set the member to the value
      memberType->Copy(data, value);
    }
  }

  static void TW_CALL GenericGetCB(void* value, void* rawData)
  {
    AntTweak::TweakGenericVar* clientData = (AntTweak::TweakGenericVar*)rawData;

    // Get the type of the member we are changing
    const TypeInfo* memberType = clientData->genericMember->Type();
    // Get a pointer to the generic object
    void* genericObject = clientData->genericSpace->GetHandles().Get(clientData->genericHandle);

    ErrorIf(genericObject == nullptr, "AntTweakBar GenericObject Variable Set", "Attempted to set a variable on an object which couldn't be found! Member: %s",
      clientData->genericMember->Name());

    void* data = (char*)genericObject + clientData->genericMember->Offset();

    if (clientData->getCB)
    {
      clientData->getCB(clientData, clientData);
    }
    else
    {
      // Use the types copy/assignment operation to set the value
      memberType->Copy(value, (char*)genericObject + clientData->genericMember->Offset());
    }

  }

  static void TW_CALL GenericGetStringCB(void* value, void* rawData)
  {
    AntTweak::TweakGenericVar* clientData = (AntTweak::TweakGenericVar*)rawData;

    // Get the type of the member we are changing
    const TypeInfo* memberType = clientData->genericMember->Type();
    // Get a pointer to the generic object
    void* genericObject = clientData->genericSpace->GetHandles().Get(clientData->genericHandle);

    ErrorIf(genericObject == nullptr, "AntTweakBar GenericObject Variable Set", "Attempted to set a variable on an object which couldn't be found! Member: %s",
      clientData->genericMember->Name());

    void* data = (char*)genericObject + clientData->genericMember->Offset();

    if (clientData->getCB)
    {
      clientData->getCB(clientData, clientData);
    }
    else
    {
      // Use the types copy/assignment operation to set the value
      *(std::string**)value = (std::string*)((char*)genericObject + clientData->genericMember->Offset());
    }

  }


  // Adds a Read/Write variable from a generic object
  void AntTweak::TBar::AddGenericVarRW(const char* name, AntTweak::engineTwType type, const Member* member, Generic* obj)
  {
#if USE_ANTTWEAKBAR
    AddGenericVarCB(name, type, member, obj);
#endif
  }

  // Adds a Read/Write callback variable from a generic object
  void AntTweak::TBar::AddGenericVarCB(const char* name, AntTweak::engineTwType type, const Member* member, Generic* obj, aTSetCB setCB, aTGetCB getCB)
  {
#if USE_ANTTWEAKBAR

    // Establish the definition list
    std::string defList;
    for (size_t i=0; i < m_definitions.size(); ++i)
    {
      defList += m_definitions[i];
    }
    for (size_t i=0; i < m_pDefinitions.size(); ++i)
    {
      defList += m_pDefinitions[i];
    }

    m_definitions.clear();

    // Establish the real type
    TwType realType = (TwType)TranslateType(type);

    // Create a clientData struct to store the client data for this variable
    TweakGenericVar* clientData = DBG_NEW TweakGenericVar(member);
    clientData->genericHandle = obj->self; // Set the handle to use
    clientData->genericSpace = obj->space; // Set the space to use (Thank god spaces are pretty static or i would flip shit)
    clientData->self = m_tweakVars.Insert(clientData);
    clientData->setCB = setCB;
    clientData->getCB = getCB;

    if (type == TW_TYPE_STDSTRING)
    {
      TwAddVarCB((TwBar*)antTweakBar, name, realType, GenericSetCB, GenericGetStringCB, clientData, defList.c_str());
    }
    else
    {
      TwAddVarCB((TwBar*)antTweakBar, name, realType, GenericSetCB, GenericGetCB, clientData, defList.c_str());
    }
#endif
  }

  void AntTweak::TBar::AddVarCB(const char* name, AntTweak::engineTwType type, AntTweak::aTSetCB setCB, AntTweak::aTGetCB getCB, void* clientData)
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

    m_definitions.clear();

    TwType realType = (TwType)TranslateType(type);

    TwAddVarCB((TwBar*)antTweakBar, name, realType, setCB, getCB, clientData, defList.c_str());
#endif
  }

  void AntTweak::TBar::AddVarRW(const char* name, AntTweak::engineTwType type, void* var)
  {
#if USE_ANTTWEAKBAR
    std::string defList;
    for (size_t i=0; i < m_definitions.size(); ++i)
    {
      defList += m_definitions[i];
    }
    for (size_t i=0; i < m_pDefinitions.size(); ++i)
    {
      defList += m_pDefinitions[i];
    }

    m_definitions.clear();

    TwType realType = (TwType)TranslateType(type);

    TwAddVarRW((TwBar*)antTweakBar, name, realType, var, defList.c_str());
#endif
  }

  void AntTweak::TBar::AddSeparator(const char* name)
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

    m_definitions.clear();

    TwAddSeparator((TwBar*)antTweakBar, name, defList.c_str());
#endif
  }

  void AntTweak::TBar::AddButton(const char* name, aTButtonCB callback, void* clientData)
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

    m_definitions.clear();

    TwAddButton((TwBar*)antTweakBar, name, callback, clientData, defList.c_str());
#endif
  }

  void AntTweak::TBar::Reset()
  {
#if USE_ANTTWEAKBAR
    TwRemoveAllVars((TwBar*)antTweakBar);
#endif
  }


  static void TweakVarDelete(void* data)
  {
    delete data;
  }

  AntTweak::TBar::~TBar()
  {
    m_tweakVars.Run(TweakVarDelete);
    name.~basic_string();
  }


  AntTweak::engineTwType AntTweak::TBar::TranslateType(AntTweak::engineTwType type)
  {
    // @TODO
    return type;
  }





}