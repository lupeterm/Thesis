#pragma once

#include <GL/gl.h>
#include <GL/glut.h>

class Lighting
{

public:
   Lighting();
   ~Lighting() {}

   void setPosition(GLfloat x, GLfloat y, GLfloat z);
   void setDifuseColor(GLfloat red, GLfloat green, GLfloat blue);
   void setSpecularColor(GLfloat red, GLfloat green, GLfloat blue);
   void setAmbientColor(GLfloat red, GLfloat green, GLfloat blue);
   void enableLighting();

private:
   GLfloat mPosition[4];
   GLfloat mDifuseColor[3];
   GLfloat mSpecularColor[3];
   GLfloat mAmbientColor[3];
};