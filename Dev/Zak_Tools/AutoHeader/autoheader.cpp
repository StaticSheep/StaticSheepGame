/*****************************************************************************/
/*!
\file   autoheader.cpp
\author Zakary Wilson
\par    email: z.wilson\@digipen.edu
\date   07/09/2014 <BR>

  \brief  
      This file contains functions for parsing through a vector of strings
      to get rid of any function that is not preceded by XXXX_API<BR>
        
      List Functions include:
        - ParseLine(std::string& string)
        - IsFunction(std::string& string)
        - IsSheepAPI(std::string& string)
*/
/*****************************************************************************/

#include <string>       // strings
#include <vector>       // vectors

/*****************************************************************************/
/*!
  \brief
    Function that reads through a string and looks for parenthesis for a 
    function prototype.
  
  \param string
    Reference to the string to parse.
    
  \return
    Returns 0 if it is not a function, and 1 if it is.
*/
/*****************************************************************************/
int IsFunction(std::string& string)
{
  size_t first;
  size_t last;
  
  first = string.find("(", 0);
  
  // if ( was found...
  if(first != std::string::npos)
  {
    last = string.find(")", first);
    
    // and if ) was found... 
    if(last != std::string::npos)
      //return that we found a function
      return 1;
  }
  
  //otherwise it's something else, so return non-function
  return 0;
}

/*****************************************************************************/
/*!
  \brief
    Function that reads through a string and looks for API in it.
  
  \param string
    Reference to the string to parse.
    
  \return
    Returns 0 if it is not an API, and 1 if it is.
*/
/*****************************************************************************/
int IsSheepAPI(std::string& string, std::string& pattern)
{
  size_t found = string.find(pattern, 0);
  
  //if API was found in the string
  if(found != std::string::npos)
    //return that this is an API function
    return 1;
  else
    //otherwise, return that it is not an API function
    return 0;
}

/*****************************************************************************/
/*!
  \brief
    Function that reads in a string, and determines if it is a function, and
    if it has API in front of it or not.
  
  \param string
    Reference to the string to parse.
    
  \return
    Returns 0 if it shouldn't be removed, and 1 if it should be removed.
*/
/*****************************************************************************/
int ParseLine(std::string& string, std::string& pattern)
{
  int functionResult = IsFunction(string);
  int apiResult = IsSheepAPI(string, pattern);
  
  //if it is a function
  if(functionResult)
  {
    //and it has API in front of it
    if(apiResult)
    {
      //keep it
      return 0;
    }
    else
      //delete it
      return 1;
  }
  else //not a function, don't care
    return 0;
}

/*****************************************************************************/
/*!
  \brief
    Function that iterates through a vector of strings and parses each line
    for functions and APIs. It then removes any element that is a function, 
    without API in front of it.
  
  \param contents
    Reference to the vector of strings

*/
/*****************************************************************************/
void ParseLines(std::vector<std::string>& contents, std::string& pattern)
{
  for(unsigned j = 0; j < contents.size(); ++j)
  {
    int result = ParseLine(contents[j], pattern);
    
    if(result)
    {
      //got rid of an element, so we need to go back one so we don't skip any
      //elements after this.
      contents.erase(contents.begin() + j--);
    }
  }
  
  return;
}