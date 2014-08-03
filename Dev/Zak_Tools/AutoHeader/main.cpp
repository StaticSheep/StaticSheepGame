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
  //check if enough arguments were given.
  
  if(argc != 4)
  {
    std::cout << "Not enough arguments, autoheader.exe takes: " << '\n'
              << "source dir, destination dir, search pattern";
    return 0;
  }
  
  //example command line
  //a C:\Users\Zakary\Desktop\SandBox\AutoHeader\ C:\Users\Zakary\Desktop\SandBox\AutoHeader\output\ SHEEP_API
  
  std::string sourcePath = argv[1];
  std::string destPath = argv[2];
  std::string keepPattern = argv[3];
  
  //look for header files
  std::string searchPattern("*.h");
  
  std::string fullPath = sourcePath + searchPattern;
  //create the entire path
  std::string fullOut;
  
  std::cout << fullPath << std::endl;
  
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
      std::cout << "Error searching directory\n";
      return -1;
  }
  
  //make a vector of strings to read in the file and parse through
  std::vector<std::string> contents;
  
  //do this while there are still files to be found
  do
  {
    std::string filePath = fileData.cFileName;
    std::cout << filePath << "\n";
    
    std::ifstream in( filePath.c_str() );
    
    //about to open a file...
    std::ofstream out;
    
    fullOut = destPath + filePath;
    
    //open the file in the output directory
    out.open(fullOut.c_str());
    
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
      
      std::cout << fullOut.c_str() << "\n";
      
      //if the file was correctly opened for writing
      if(out)
      {
        //iterator for going through the vector entirely
        std::vector<std::string>::iterator it;
        
        //go through the entire vector and put in every string
        for( it = contents.begin(); it != contents.end(); ++it)
        {
          //shove it in there
          out << contents[i++];
        }
      }
      
      //clear our vector
      contents.clear();
      //and close the file
      in.close();
      
    }
    else
    {
      std::cout << "Problem opening file " 
                << fileData.cFileName << "\n";
    }
  }
  //while there are more files, open the next one
  while( FindNextFile(hFind, &fileData) != 0 );
  
  
  //all done, gtfo
  return 0;
}
