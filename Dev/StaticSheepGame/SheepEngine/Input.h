#include <Windows.h>

struct Pos
{
  float x;
  float y;
};

struct Control
{
  bool IsPressed;
  bool IsDown;
  bool IsReleased;
};

class MouseInput
{
public:

  void Update(void);
  void GetWorldPosition(void);
  void GetScreenPosition(void);
  

private:

  Control LMB;
  Control RMB;
  Pos _position;

  void _UpdateButton(MSG* msg);
  void _UpdateMove(MSG* msg);

};

class InputManager
{
public:

  MouseInput Mouse;
};