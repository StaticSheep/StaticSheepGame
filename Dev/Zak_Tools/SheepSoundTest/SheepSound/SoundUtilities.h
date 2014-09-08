#include <string>

namespace SoundUtility
{
  // enum for defining where to look for files
  enum SourceType
  {
    TYPE_AUDIO,
    TYPE_GUIDs
  };

  // function for grabbing the correct directory
  std::string SourcePath(const std::string file, SourceType type);
}