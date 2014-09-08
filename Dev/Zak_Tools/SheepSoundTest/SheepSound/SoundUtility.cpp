#include "SoundUtilities.h"

namespace SoundUtility
{
  std::string SourcePath(const std::string file, SourceType type)
  {
    switch(type)
    {
      case TYPE_AUDIO : return "Assets\\Desktop\\" + file;
      case TYPE_GUIDs : return "Assets\\" + file;
      default : return "go away";
    }
  }
}