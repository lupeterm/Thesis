#pragma once


#include "SwissArmyKnife.h"


class sample_app : public Canvas {
public:
   sample_app(int width, int height);

   void update();
   void render();
   void mouseMotion( int x, int y );
   void mouseClick( int button, int state, int x, int y );
   void keyboardDown( unsigned char key, int x, int y );
   void keyboardUp( unsigned char key, int x, int y );
   void reshape( int w, int h );
   void specialKeyboardDown( int key, int x, int y );

   bool hold;
   Lighting * myLighting;
   CamManager * myCamera;

};
