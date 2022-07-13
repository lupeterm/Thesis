#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

template<class T>
std::string toString(T rhs)  {
   std::stringstream stream;
   stream << rhs;
   return stream.str();
}

template<class T>
T fromString(const std::string &str)  {
   std::istringstream stream (str);
   T classType;
   stream >> classType;
   return classType;
}

std::string trim(const std::string &str);