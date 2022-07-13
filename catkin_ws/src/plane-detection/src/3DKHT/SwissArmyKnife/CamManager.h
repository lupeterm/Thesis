#pragma once

#include <GL/gl.h>
#include <GL/glut.h>

#include <vector>
#include "Vector4d.h"
#include "Matrix4d.h"

class CamManager
{

public:
   CamManager(Vector4d eye, Vector4d center);
   ~CamManager(void) {}

   void drawAxis(GLfloat size);
   void drawGrid(GLfloat size, int slices);
   void onMouseClick(int button, int x, int y);
   void onMouseHold(int x, int y);
   void onKeyboard(char key, bool state);
   void translateCamera(const Vector4d v);
   void rotateCameraByCenter(Vector4d eye, Vector4d up, const Matrix4d &m);
   void rotateCameraByEye(Vector4d eye, Vector4d up, const Matrix4d &m);
   void render(void);

   Vector4d _eye;
   Vector4d _center;
   Vector4d _up;

private:
   Vector4d c_eye;
   Vector4d c_center;
   Vector4d c_up;
   Vector4d lastClickPos;
   std::vector<bool> keys;

   enum
   {
      front = 0,
      back = 1,
      right = 2,
      left = 3,
      turnright = 4,
      turnleft = 5,
      wheel = 6
   };
};
