#pragma once

#include "Canvas.h"

class OpenGL {
public:
   /*
   OpenGL(Canvas * canvas) {
      _canvas = canvas;
   }*/
   
   static void reshape(int w, int h);
   //static void text(int x, int y, char *t);
   static void keyboardDown(unsigned char key, int x, int y);
   static void keyboardUp(unsigned char key, int x, int y);
   static void specialKeyboard(int key, int x, int y);
   static void specialKeyboardUP(int key, int x, int y);
   static void mouseClick(int button, int state, int x, int y);
   static void display();
   static void mouseMotion(int x, int y);

   static void init(Canvas * canvas);    

   static int _width;
   static int _height;

   static Canvas * _canvas;
   
};



