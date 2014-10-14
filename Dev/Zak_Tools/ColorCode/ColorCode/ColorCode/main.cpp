#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>

int filesParsed;
int filesOpened;
std::string parse(const char * filename);
void MakeBMP(std::string& Thoth);
void MakeWav(std::string andFriends);
int main(int argc, char* argv[])
{
   WIN32_FIND_DATA fileData;
  
  //handle to the file
  HANDLE hFind;
  
  std::string GrandMaster;
  //HANDLE hFindOut;

  //find any header file in the current directory
  hFind = FindFirstFile("TheAllFile\\*.cpp", &fileData );
  
  //if a file was not found
  if( hFind == INVALID_HANDLE_VALUE )
  {
      //exit out
      std::cout << "Error searching directory.\n";
      return -1;
  }
  
  //make a vector of strings to read in the file and parse through
  //std::vector<std::string> contents;
  
  //do this while there are still files to be found
  do
  {
    std::string filePath = fileData.cFileName;
    
    //std::ifstream in( filePath.c_str() );
    GrandMaster += parse((std::string("TheAllFile\\") + filePath).c_str());
  }
  //while there are more files, open the next one
  while( FindNextFile(hFind, &fileData) != 0 );
  MakeWav(GrandMaster);
}

std::string parse(const char * filename)
{
  filesParsed++;
  std::ifstream file(filename);
  if(file.is_open())
  {
    filesOpened++;
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char * buffer = new char[size];
    file.read(buffer, size);
    std::string out(buffer);
    delete [] buffer;
    return out;
  }
  return std::string();
}

void MakeBMP(std::string& Thoth)
{

  // so we do some things
  int totalPixels = Thoth.size() / 3;

  int H = sqrt(totalPixels), W = sqrt(totalPixels), S = (3*W) + (W % 4);

  char *data = new char[S*H];
  memset(data,0,S*H);
  memcpy(data, Thoth.c_str(), Thoth.size());

  // write bitmap file header
  char header[54];
  memset(header,0,54);
  unsigned size = S*H;
  header[0] = 'B';  header[1] = 'M';
  *reinterpret_cast<unsigned*>(header+2) = 54u + size;
  *reinterpret_cast<unsigned*>(header+10) = 54u;
  *reinterpret_cast<unsigned*>(header+14) = 40u;
  *reinterpret_cast<int*>(header+18) = W;
  *reinterpret_cast<int*>(header+22) = H;
  *reinterpret_cast<unsigned short*>(header+26) = 1u;
  *reinterpret_cast<unsigned short*>(header+28) = 24u;
  *reinterpret_cast<unsigned*>(header+34) = size;
  std::fstream out("PraiseBeToThoth.bmp",std::ios_base::binary|std::ios_base::out);
  out.write(header,54);
  
  char* reader = data;

  //for(int i = 0; i < S*H; ++i)
  //{
  //  *reader = *reader * 20;
  //  ++reader;
  //}

  out.write(reinterpret_cast<char*>(data),size);

  delete[] data;
  return;
}

void MakeWav(std::string andFriends)
{
    // so we do some things
  char *data = new char[andFriends.size()];
  memset(data,0,andFriends.size());
  memcpy(data, andFriends.c_str(), andFriends.size());

  // write bitmap file header
  char header[54];
  memset(header,0,54);
  unsigned size = andFriends.size();
  header[0] = 'R';  header[1] = 'I';
  header[2] = 'F';  header[3] = 'F';
  header[7] = 'W';  header[8] = 'A';
  header[9] = 'V';  header[10] ='E';
  *reinterpret_cast<unsigned*>(header+4) = size;
  *reinterpret_cast<char*>(header+12) = 'f';
  *reinterpret_cast<char*>(header+13) = 'm';
  *reinterpret_cast<char*>(header+14) = 't';
  *reinterpret_cast<unsigned*>(header+15) = (char)20;
  *reinterpret_cast<int*>(header+16) = 16u;
  *reinterpret_cast<int*>(header+20) = (short)1;
  *reinterpret_cast<unsigned short*>(header+22) = (short)1;
  *reinterpret_cast<unsigned short*>(header+24) = 44100u;
  *reinterpret_cast<unsigned*>(header+28) = 88200u;
  *reinterpret_cast<unsigned*>(header+32) = 2u;
  *reinterpret_cast<unsigned*>(header+34) = 16u;
  *reinterpret_cast<unsigned*>(header+36) = 'd';
  *reinterpret_cast<unsigned*>(header+37) = 'a';
  *reinterpret_cast<unsigned*>(header+38) = 't';
  *reinterpret_cast<unsigned*>(header+39) = 'a';
  *reinterpret_cast<unsigned*>(header+40) = 2 * size;
  std::fstream out("PraiseBeToThoth.wav",std::ios_base::binary|std::ios_base::out);
  out.write(header,54);
  
  char* reader = data;

  //for(int i = 0; i < S*H; ++i)
  //{
  //  *reader = *reader * 20;
  //  ++reader;
  //}

  out.write(reinterpret_cast<char*>(data),size);

  delete[] data;
  return;
}