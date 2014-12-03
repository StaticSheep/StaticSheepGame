/*****************************************************************
Filename: Message.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

// Messages proves system->system communication in the SheepEngine
// They are sent to every single system running regardless of the
// relevancy of the message to the system.

namespace Framework
{

	// Base message class. You can make your own message types by deriving from this
	// and then added an ID to the MessageIDType enum
	class Message
	{
   public:
    enum MessageType
    {
      None,
      GFXDeviceInit,
      PostDraw,
      GUIDraw,
      PostGUIDraw,
      WindowResize,
      WindowMinimize,
      WindowRestore,
      CameraChange,
      Pause,
      Quit,
    };


		Message(MessageType id) : MessageId(id){};
		MessageType MessageId;
		virtual ~Message(){};
	};

  class ResizeMessage : public Message
  {
  public:
    ResizeMessage(float w, float h)
      : Message(WindowResize),
      width(w),
      height(h)
    {
      
    }

    float width;
    float height;
  };
}