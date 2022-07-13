#pragma once

#include <fstream>

class File {
public:
   File(){}

   bool open(std::string filepath)
   {
      _file.open(filepath);
      return _file.is_open();
   }
   bool close()
   {
      _file.close();
      return !_file.is_open();
   }
   void write(std::string words) 
   {
      _file << words.c_str();
   }
   std::ofstream & getFile() 
   {
      return _file;
   }
   bool isOpen() 
   {
      return _file.is_open();
   }

private:

   std::ofstream _file;


};