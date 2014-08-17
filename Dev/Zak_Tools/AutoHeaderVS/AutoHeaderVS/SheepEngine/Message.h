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
	namespace Mid // Message ID's
	{
		enum MessageType
		{
			None,
      Pause,
			Quit,
		};
	}

	// Base message class. You can make your own message types by deriving from this
	// and then added an ID to the MessageIDType enum
	class Message
	{
	public:
		Message(Mid::MessageType id) : MessageId(id){};
		Mid::MessageType MessageId;
		virtual ~Message(){};
	};
}