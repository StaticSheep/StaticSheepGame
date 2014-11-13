/*****************************************************************
Filename: AntTweak.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "systems/System.h"

namespace Framework
{
  class AntTweakModule;

  namespace AntTweak
  {

    typedef void(*aTSetCB)(const void* value, void* clientData);
    typedef void(*aTGetCB)(void* value, void* clientData);
    typedef void(*aTButtonCB)(void* clientData);

    class TBar
    {
    public:

      //============================================== Definitions ====================================================//

      // Adds a definition to use when creating buttons/vars
      void AddDefinition(const char* definition, bool persist=false);
      // Clears the vector of persistent definitions
      void ClearDefinitions(void);
      // Sets the label for the next bar/button
      void DefineLabel(const char* label);
      // Sets the help message for the next button/var
      void DefineHelpMessage(const char* message);
      // Sets the group for the next button/var
      void DefineGroup(const char* group);
      // Sets if the next button/var is visible
      void DefineVisible(bool visible);
      // Sets a min/max for the next var
      // If the next value is an integer, then it will truncate
      void DefineMinMax(float min, float max);
      // Sets a step value for the variable if it is a slider
      void DefineStep(float step);
      // Sets a keyboard shortcut for a button
      // Refer to http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:varparamsyntax for help
      void DefineKeyShortcut(const char* key);
      // Sets a keyboard shortcut for Increment and Decrement
      void DefineIncDecShortcut(const char* inc, const char* dec);
      // Sets a word description for a boolean value
      void DefineBool(const char* isTrue, const char* isFalse);

      //============================================== Bar Setters ====================================================//

      // Opens or closes a variable/button grouping in a bar
      void SetGroupOpened(const char* name, bool isOpen);
      // Sets a bar label
      void SetLabel(const char* name);
      // Defines the help message associated with the bar
      void SetHelpLabel(const char* name);
      // Set Bar Color
      void SetColor(int red, int green, int blue);
      // Sets the alpha-value of the bar
      void SetAlpha(int alpha);
      // Sets the position of the bar
      void SetPos(int x, int y);
      // Sets the size of the bar
      void SetSize(int x, int y);
      // Sets group heirarchy
      void SetGroupParent(const char* child, const char* parent);
      // Sets the bar refresh rate
      void SetRefreshRate(float rate);
      // Sets whether the bar is iconified or not
      void SetIconify(bool icon);
      // Sets whether the bar is movable or not
      void SetMovable(bool canMove);
      // Sets whether the bar is re-sizable
      void SetResizable(bool canResize);
      // Sets the size of the value fields
      void SetValueWidth(int w);
      

      //============================================== Content ====================================================//

      // Creates a variable with a callback function for setting and getting
      void AddVarCB(const char* name, AntTweak::engineTwType type, aTSetCB setCB, aTGetCB getCB, void* clientData);
      // Creates a Read/Write variable
      void AddVarRW(const char* name, AntTweak::engineTwType type, void* var);
      // Creates a Read/Write variable for a generic object
      void AddGenericVarRW(const char* name, AntTweak::engineTwType type, const Member* member, unsigned extraOffset, Generic* obj);
      // Creates a variable with a callback function for setting and getting for a Generic Object
      void AddGenericVarCB(const char* name, AntTweak::engineTwType type, const Member* member, unsigned extraOffset, Generic* obj, Function setCB, Function getCB);
      // Adds a seperator into the bar and names it something
      void AddSeparator(const char* name);
      // Adds a button to the bar
      void AddButton(const char* name, aTButtonCB callback, void* clientData);

      // Resets a AntTweakBar
      void Reset(void);

      std::string name;
      Handle self;

      TBar() {};
      TBar(const char* name) : name(name) {};
      ~TBar();


      // Translates a AntTweak::TwType (Extended Enum) into a TwType enum
      static AntTweak::engineTwType TranslateType(AntTweak::engineTwType type);


    private:
      
      // Pointer to the actual bar
      void* antTweakBar;
      // List of AntTweakBar definitions
      std::vector<std::string> m_definitions;
      // List of persistent AntTweakBar definitions
      std::vector<std::string> m_pDefinitions;
      // Handles of tweakvars
      HandleManager m_tweakVars;
      // Remove on next update?
      bool toRemove;

      friend class AntTweakModule;
    };

    

  }

  class AntTweakModule : public ISystem
  {
  public:
    AntTweakModule();
    ~AntTweakModule();

    void Initialize(void);

    // Update per frame
    void Update(float dt);

    void ReceiveMessage(Message& msg);

    void Shutdown();

    // Creates a new bar of given name and returns a BarContiner*
    AntTweak::TBar* CreateBar(const char* name);
    // Gets a TBar from a handle
    AntTweak::TBar* GetBar(Handle barHandle);

    void RemoveBar(AntTweak::TBar* bar);
    void RemoveAllBars(void);

    virtual std::string GetName() {return "AntTweakBar";};

  private:

    void RemoveBars(void);
    
    // Handle manager for AntTweak TBars
    HandleManager m_handles;
    // Object allocator for BarContainters
    ObjectAllocator m_bars;
  };

  extern AntTweakModule* ATWEAK;
}
