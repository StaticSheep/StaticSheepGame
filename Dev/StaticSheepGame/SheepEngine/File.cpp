/*****************************************************************
Filename: File.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include <stdio.h>
#include <stdarg.h>

namespace Framework
{
  // Super hacky thing found at: http://mrpt.googlecode.com/svn-history/r2406/trunk/libs/base/src/system/os.cpp
  // This was shamelessly borrowed from Randy Gaul because there is no way i would ever be able to write this myself
  int vfscanf( FILE *file, const char *format, va_list args )
  {
    int count = 0;
    const char *p = format;

    for(;;)
    {
      char c = *(p++);
      if (c == 0)
        break;

      // Count number of specifiers to pass to fscanf
      if (c == '%' && *p != '%')
        ++count;
    }

    if (count <= 0)
      return 0;

    int retVal;

    // copy stack pointer
    _asm
    {
      mov esi, esp;
    }

    const int paramSize = sizeof( void * );
    DISABLE_WARNING(4996)

    // push variable parameters pointers on stack
    for (int i = ((int)count) - 1; i >= 0; --i)
    {
      assert( paramSize == 4 ); // The 4 a few lines down must be
      // modified to match the size of param_size
      _asm
      {
        mov eax, dword ptr[i];
        mov ecx, dword ptr[args];
        mov edx, dword ptr[ecx + eax * 4]; // 4 as in the size of void *
        push edx;
      }
    }

    
    int stackAdvance = (int)((2 + count) * paramSize);
      _asm
    {
      // now push on the fixed params
      mov eax, dword ptr [format];
      push eax;
      mov eax, dword ptr [file];
      push eax;

      // call fscanf, and move the result into ret_val
      call dword ptr [fscanf];
      mov retVal, eax;

      // restore stack pointer
      mov eax, dword ptr[stackAdvance];
      add esp, eax;
      //mov esp, esi;
    }
    END_DISABLE()

    return retVal;
  }

  File::File() : fp(nullptr) {}
  File::File(FILE* fileptr) : fp(fileptr) {}
  File::~File() {Close();};

  void File::Open(const char* name, FileAccess::Type access)
  {
    DISABLE_WARNING(4996)
    switch (access)
    {
    case FileAccess::Read:
      fp = fopen(name, "r");
      break;
    case FileAccess::Write:
      fp = fopen(name, "w");
      break;
    case FileAccess::Append:
      fp = fopen(name, "a");
      break;
    }
    END_DISABLE()
    
    assert(fp); // make sure its not null

  }

  void File::Close()
  {
    if (fp)
    {
      fclose(fp);
    }
  }

  /// <summary>
  /// Writes to the file
  /// </summary>
  /// <param name="format">The format.</param>
  /// <param name="">what to write</param>
  void File::Write(const char* format, ...)
  {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
  }

  /// <summary>
  /// Reads from the file
  /// </summary>
  /// <param name="format">The format.</param>
  /// <param name="">var args</param>
  void File::Read(const char* format, ...)
  {
    va_list args;
    va_start(args, format);
    vfscanf(fp, format, args);
    va_end(args);
  }

  void File::SeekFromBeginOfFile(int offset)
  {
    fseek(fp, offset, SEEK_SET);
  }

  void File::SeekFromEndOfFile(int offset)
  {
    fseek(fp, offset, SEEK_END);
  }

  void File::SeekByOffset(int offset)
  {
    fseek(fp, offset, SEEK_CUR);
  }

  bool File::Validate(void) const
  {
    return fp && !feof(fp) ? true : false;
  }

  std::string File::GetLine(char delim)
  {
    std::string temp;
    char c;

    for(;;)
    {
      c = static_cast<char>(fgetc(fp));

      if(feof( fp ))
        break;
      else if(c == delim)
        break;

      temp += c;
    }

    return std::move( temp );
  }

  std::string File::GetLine(const char* delims)
  {
    std::string temp;

    char c;

    for(;;)
    {
      c = static_cast<char>(fgetc(fp));

      if (feof( fp ))
        break;
      for (const char *temp_delim = delims; *temp_delim; ++temp_delim)
        if (c == *temp_delim)
          return std::move( temp );
      temp += c;
    }

    return std::move( temp );
  }

}