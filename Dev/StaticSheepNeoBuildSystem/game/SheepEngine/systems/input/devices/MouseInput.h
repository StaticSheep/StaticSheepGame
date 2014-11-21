


namespace Framework
{
  class MouseInput
  {
  public:

    // default constructor/destructor
    MouseInput();
    ~MouseInput();

    // basic functions
    void Initialize(void);
    void Update(void);

    // getters
    Vec2 GetWorldPosition(void);
    Vec2 GetScreenPosition(void);
  
    // checks the state of the buttons
    bool ButtonPressed(unsigned int button) const;
    bool ButtonDown(unsigned int button) const;
    bool ButtonReleased(unsigned int button) const;

  private:

    // the previous frame's state
    bool _previousState[3];
    // the current frame's state
    bool _currentState[3];

    // positions
    Vec2 _screenPosition;
    Vec2 _worldPosition;
    void GetMsg(void* msg);

    void _UpdateButton(unsigned int, bool state);
    void _UpdateMove(void* msg);

    void _ScreenToWorld();
    
    // let the manager touch our privates
    friend class InputManager;

  };
}