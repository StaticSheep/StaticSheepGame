/*****************************************************************************/
/*!
\file   main.cpp
\author Zakary Wilson
\par    email: z.wilson\@digipen.edu
\date   07/09/2014 <BR>

  \brief  
      This file contains main, that opens all header files in the current
      directory, removes private functions, and outputs a new header file
      to an output folder.<BR>
*/
/*****************************************************************************/

#include <windows.h>    // handles, FindFirstFile()
#include <iostream>     // cout, errors
#include <fstream>      // file i/o
#include <string>       // std::string
#include <vector>       // std::vector

// function prototype
void ParseLines(std::vector<std::string>& contents, std::string& pattern);

/*****************************************************************************/
/*!
  \brief
    Start of the program. Opens any header files, parses it, and outputs
    a new header.
*/
/*****************************************************************************/
int main(int argc, char* argv[])
{

  char wPath[ MAX_PATH ];
  GetModuleFileName( NULL, wPath, MAX_PATH );

  wPath[strlen(wPath) - strlen(argv[0])] = 0;

  std::cout << "Working Path: " << wPath << "\n";

  //check if enough arguments were given.
  
  if(argc != 4)
  {
    std::cout << "Not enough arguments, autoheader.exe takes: " << '\n'
              << "source dir, destination dir, search pattern";
    return 0;
  }
  
  //example command line
  //a C:\Users\Zakary\Desktop\SandBox\AutoHeader\ C:\Users\Zakary\Desktop\SandBox\AutoHeader\output\ SHEEP_API
  
  std::string sourcePath;// = wPath;
  sourcePath += argv[1];

  std::cout << "SourcePath: " << sourcePath << "\n";

  std::string destPath;// = wPath;
  destPath += argv[2];

  std::cout << "DestPath: " << destPath << "\n";

  std::string keepPattern = argv[3];
  
  //look for header files
  std::string searchPattern("*.h");
  
  std::string fullPath = sourcePath + searchPattern;
  //create the entire path
  std::string fullOut;
  
  //std::cout << fullPath << std::endl;
  
  //stores the file data
  WIN32_FIND_DATA fileData;
  
  //handle to the file
  HANDLE hFind;
  
  //HANDLE hFindOut;

  //find any header file in the current directory
  hFind = FindFirstFile( fullPath.c_str(), &fileData );
  
  //if a file was not found
  if( hFind == INVALID_HANDLE_VALUE )
  {
      //exit out
      std::cout << "Error searching directory.\n";
      return -1;
  }
  
  //make a vector of strings to read in the file and parse through
  std::vector<std::string> contents;
  
  //do this while there are still files to be found
  do
  {
    std::string filePath = sourcePath + fileData.cFileName;
    
    std::ifstream in( filePath.c_str() );

    //about to open a file...
    std::ofstream out;
    
    fullOut = destPath + fileData.cFileName;

    WIN32_FIND_DATA outFileData;
    HANDLE outHFind;

    outHFind = FindFirstFile(fullOut.c_str(), &outFileData);

    bool updateFile = true;

    if (outHFind != INVALID_HANDLE_VALUE)
    {
      int fileNewer = CompareFileTime(&fileData.ftLastWriteTime, &outFileData.ftLastWriteTime);

      if (fileNewer < 0)
        updateFile = false;
    }

    if(!updateFile)
      continue;

    std::cout << "Updating: " << filePath << "\n";

    //if the file was opened
    if(in)
    {
      int i = 0;
      
      //search through the file
      while(!in.eof())
      {
        std::string str;
        std::getline(in, str);
        
        //put every line into the vector
        contents.push_back(str);
      }
      
      //read through the entire file, getting rid of
      //non API functions
      ParseLines(contents, keepPattern);
      
      //std::cout << fullOut.c_str() << "\n";

      //open the file in the output directory
      out.open(fullOut.c_str());
      
      //if the file was correctly opened for writing
      if(out)
      {
        //iterator for going through the vector entirely
        std::vector<std::string>::iterator it;
        
        //go through the entire vector and put in every string
        for( it = contents.begin(); it != contents.end(); ++it)
        {
          //shove it in there
          out << contents[i++] << "\n";
        }
      }
      else
      {
        std::cout << "Unable to open output file!\n";
      }
      
      //clear our vector
      contents.clear();
      //and close the file
      in.close();
      
    }
    else
    {
      DWORD dw = GetLastError();
      LPVOID lpMsgBuf;

      FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );


      std::cout << "Problem opening file " 
                << fileData.cFileName << "\n";
      std::cout << (LPSTR)lpMsgBuf << "\n";

      LocalFree(lpMsgBuf);
    }
  }
  //while there are more files, open the next one
  while( FindNextFile(hFind, &fileData) != 0 );
  
  
  //all done, gtfo
  return 0;
}
