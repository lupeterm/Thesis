#include <cstdio>
#include <cstdlib>

#include "SwissArmyKnife.h"
#include "sample_app.h"
#include "settings.h"


int main(void) {  

#ifdef debugplanes
   sample_app * app = new sample_app(800,800);
#endif
#ifndef debugplanes
   sample_app * app = new sample_app(100,100);
#endif
   
   OpenGL::init(app);

   return 0;
}