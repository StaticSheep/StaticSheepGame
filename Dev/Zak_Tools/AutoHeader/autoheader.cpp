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
        - IsTemplate(std::string& string)
        - IsSheepAPI(std::string& string)
*/
/*****************************************************************************/

#include <string>       // strings
#include <vector>       // vectors

enum LineType
{
  None,
  Namespace,
  Template,
  FunctionDefinition,
  FunctionPrototype,
  InFunction,
  ClassOrStruct
};

int Gprotectionlevel = 0;

LineType GlastLine = Namespace;

int GbracketLevel = 0;

int GstartFunctionlevel = 0;

bool GinFunction = false;

bool GremoveFunction = false;

bool GmacroDelete = false;

/*****************************************************************************/
/*!
  \brief
    Function that reads through a string and looks for parenthesis for a 
    function prototype.
  
  \param string
    Reference to the string to parse.
    
  \return
    Returns 0 if it is not a function
    1 if it is a prototype
    2 if it is a definition
*/
/*****************************************************************************/
int IsFunction(std::string& string)
{
  size_t first;
  size_t last;
  size_t scolon;
  
  first = string.find("(", 0);
  
  // if ( was found...
  if(first != std::string::npos)
  {
    last = string.find(")", first);
    
    // and if ) was found... 
    if(last != std::string::npos)
    {
      scolon = string.find(";", last);

      if(scolon != std::string::npos)
        //return that we found a function
        return 1;
      else
        return 2;
      
    }
  }
  
  //otherwise it's something else, so return non-function
  return 0;
}

/*****************************************************************************/
/*!
  \brief
    Function that checks to see if the name is a namespace
  
  \param string
    Reference to the string to parse.
    
  \return
    True if namespace, false if not
*/
/*****************************************************************************/
bool IsNamespace(std::string& string)
{
  size_t id;

  id = string.find("namespace ", 0);
  if(id != std::string::npos)
    return true;

  return false;
}

/*****************************************************************************/
/*!
  \brief
    Function that checks to see if the string is a class or struct declaration
  
  \param string
    Reference to the string to parse.
    
  \return
    True if class/struct, false if not
*/
/*****************************************************************************/
bool IsStructOrClass(std::string& string)
{
  size_t id;
  
  id = string.find("struct ", 0);
  if(id != std::string::npos)
    return true;

  id = string.find("class ", 0);
  if(id != std::string::npos)
    return true;

  return false;
}


int CheckProtection(std::string& string)
{
  size_t id;

  id = string.find("public:", 0);
  if(id != std::string::npos)
    return 0;

  id = string.find("protected:", 0);
  if(id != std::string::npos)
    return 1;

  id = string.find("private:", 0);
  if(id != std::string::npos)
    return 2;

  return -1;
}


/*****************************************************************************/
/*!
  \brief
    Function that finds brackets and increases/decreases the level
  
  \param string
    Reference to the string to parse.
    
  \return
    0 if no brackets
    1 if open bracket
    -1 if close bracket
*/
/*****************************************************************************/
int CheckBrackets(std::string& string)
{
  size_t open;
  size_t close;
  int level = 0;

  open = string.find("{", 0);
  close = string.find("}", 0);

  if (open != std::string::npos)
    ++level;
  if (close != std::string::npos)
    --level;

  return level;
}

/*****************************************************************************/
/*!
  \brief
    Function that reads through a string and looks for the template keyword
  
  \param string
    Reference to the string to parse.
    
  \return
    Returns 0 if it is a template function, and 1 if it is.
*/
/*****************************************************************************/
int IsTemplate(std::string& string)
{
  size_t found;

  found = string.find("template<", 0);

  // if "template<" was found
  if(found != std::string::npos)
    return 1;

  found = string.find("template <", 0);

  // if "template<" was found
  if(found != std::string::npos)
    return 1;

  return 0;
}

bool IsMacro(std::string& string)
{
  size_t id;

  id = string.find("#", 0);
  if(id != std::string::npos)
    return true;

  if(GmacroDelete)
  {
    id = string.find("\\", 0);
    if(id != std::string::npos)
      return true;
  }

  return false;
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
    0 if it shouldn't be removed  
    1 if it should be removed
    2 if the previous line and the current line should be removed
*/
/*****************************************************************************/
int ParseLine(std::string& string, std::string& pattern)
{
  int functionResult = IsFunction(string);
  int templateResult = IsTemplate(string);
  int apiResult = IsSheepAPI(string, pattern);
  int level = CheckBrackets(string);
  int protectionLevel = CheckProtection(string);

  bool namespaceResult = IsNamespace(string);
  bool classResult = IsStructOrClass(string);
  bool macroResult = IsMacro(string);

  if (macroResult)
  {
    GmacroDelete = true;
    return 0;
  }
  else if(GmacroDelete == true)
  {
    GmacroDelete = false;
    return 0;
  }
  else
  {
    GmacroDelete = false;
  }
  


  Gprotectionlevel = protectionLevel;

  if(namespaceResult)
  {
    GlastLine = Namespace;
    return 0;
  }

  if(classResult)
  {
    GlastLine = ClassOrStruct;
    return 0;
  }

  GbracketLevel += level;

  // Check to see the last line was a function defintion
  // and if it was and we are on an open bracket then that
  // means we are not inside of a function
  if(GlastLine == FunctionDefinition && level > 0 && GinFunction == false)
  {
    GlastLine = InFunction;
    GinFunction = true;

    GstartFunctionlevel = GbracketLevel - 1;

    if (GremoveFunction)
      return 1;
    else
      return 0;
  }

  if( (GlastLine == InFunction || GlastLine == FunctionDefinition) && level < 0 && GstartFunctionlevel == GbracketLevel)
  {
    GlastLine = None;
    GinFunction = false;

    if (GremoveFunction)
      return 1;
    else
      return 0;
  }

  if(GinFunction)
  {
    if (GremoveFunction)
    {
      GlastLine = InFunction;
      return 1;
    }
    GlastLine = InFunction;
    return 0;
  }
  

  if(templateResult && functionResult == 0)
  {
    // Template line found, keep it for now
    GlastLine = Template;
    return 0;
  }
  
  //if it is a function
  if(functionResult == 1)
  {
    //and it has API in front of it
    if(apiResult)
    {
      GlastLine = FunctionPrototype;
      //keep it
      return 0;
    }
    else
    {
      // If the last line was a template
      if (GlastLine == Template)
      {
        // We need to remove both this line and the previous line
        return 2;
      }
      else
        // delete it
        return 1;
    }
  }
  else if(functionResult == 2)
  {
    if(apiResult)
    {
      GlastLine = FunctionDefinition;
      return 0;
    }
    else
    {
      GremoveFunction = true;
      GlastLine = FunctionDefinition;
      return 2;
    }
    
  }
  else //not a function, don't care
  {
    GlastLine = None;
    return 0;
  }
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
    
    if(result == 1)
    {
      //got rid of an element, so we need to go back one so we don't skip any
      //elements after this.
      contents.erase(contents.begin() + j--);
    }
    else if(result == 2)
    {
      if (j == 0)
      {
        contents.erase(contents.begin() + j--);
        return;
      }
      // Get rid of the current element and the one before it
      contents.erase(contents.begin() + --j);
      contents.erase(contents.begin() + j--);
    }
  }
  
  return;
}