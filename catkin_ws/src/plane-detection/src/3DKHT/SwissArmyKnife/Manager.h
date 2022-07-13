#pragma once

#include "Canvas.h"

class Manager {
public:

   Manager(){}

   void addCanvas(Canvas * c)
   {
      _canvas = c;
   }


   Canvas * _canvas;



};