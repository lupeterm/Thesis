#pragma once

class Canvas {
public:
   Canvas(int width, int height) {
      _width = width;
      _height = height;
   }

   virtual void update(){}
   virtual void render(){}

   virtual void mouseMotion(int x, int y){}
   virtual void mouseClick(int button, int state, int x, int y){}
   virtual void keyboardDown(unsigned char key, int x, int y){}
   virtual void keyboardUp(unsigned char key, int x, int y){}
   virtual void reshape(int w, int h){}
   virtual void specialKeyboardDown(int key, int x, int y){}
   virtual void specialKeyboardUp(int key, int x, int y){}

   int _width;
   int _height;

};

