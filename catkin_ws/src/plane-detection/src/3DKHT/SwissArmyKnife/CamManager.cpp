#include "CamManager.h"
#include "Mathematic.h"
#include "GL/glu.h"

CamManager::CamManager(Vector4d eye, Vector4d center)  {
   c_eye =  _eye = eye;
   c_center = _center = center;
   c_up = _up = Vector4d(0.0,0.0,1.0);
   keys.resize(10,false);
}

void CamManager::rotateCameraByCenter(Vector4d theEye, Vector4d theUp, const Matrix4d &m) {
   theEye -= this->_center;
   this->_eye = m * theEye;
   this->_up =  m * theUp;
   this->_eye += this->_center;
}

void CamManager::rotateCameraByEye(Vector4d theCenter, Vector4d theUp, const Matrix4d &m) {
   theCenter -= this->_eye;
   this->_center = m * theCenter;
   this->_up =  m * theUp;
   this->_center += this->_eye;
}

void CamManager::translateCamera(const Vector4d v) {
   _eye += v;
   //_center += v;
}



void CamManager::drawGrid(GLfloat size, int slices) {
   //glEnable(GL_QUAD_STRIP);
   glPushMatrix();
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glLineWidth(2.0);

   
   
   glColor3d(0.8,0.8,0.8);
   double size2 = size/2.0;

   for (double x = -size2 ; x <= size2; x+= size/(double)slices)
   {
      glBegin(GL_QUAD_STRIP);
      for (double y = -size2 ; y < size2; y+= size/(double)slices)
      {
         glVertex3d(x,y,0.0); glVertex3d(x+size/(double)slices,y,0.0);
      }
      glEnd();
   }

   glPopMatrix();
   
}

void CamManager::drawAxis(GLfloat size) {
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x000F);
   glBegin(GL_LINES);
      glColor3f(1.0,0.0,0.0);
      glVertex3f( size,0.0,0.0);
      glVertex3f(-size,0.0,0.0);
      glColor3f(0.0,1.0,0.0);
      glVertex3f(0.0, size,0.0);
      glVertex3f(0.0,-size,0.0);
      glColor3f(0.0,0.0,1.0);
      glVertex3f(0.0,0.0, size);
      glVertex3f(0.0,0.0,-size);
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}

void CamManager::onKeyboard(char key, bool state) {
   switch (key) {
      case 'w': keys[front]     = state; break;
      case 's': keys[back]      = state; break;
      case 'q': keys[left]      = state; break;
      case 'e': keys[right]     = state; break;
      case 'a': keys[turnleft]  = state; break;
      case 'd': keys[turnright] = state; break;
   }
}

void CamManager::render(void) {
   Vector4d forward = (_center - _eye);
   Vector4d r = (_up * forward).Normalized();
   Vector4d up = (r * forward).Normalized();   


   
   if (keys[front])     { if (forward.GetLength() > 0.2) translateCamera(forward/200.0); }
   if (keys[back])      { translateCamera(-forward/30.0); }
   if (keys[turnleft])  { _eye -= r*(forward.GetLength()/1000.0); } 
   if (keys[turnright]) { _eye += r*(forward.GetLength()/100.0); }
   if (keys[left])      { rotateCameraByCenter(_eye, _up, Matrix4d::Rotation(-0.005, Vector4d(0.0,0.0,1.0))); }
   if (keys[right])     { rotateCameraByCenter(_eye, _up, Matrix4d::Rotation(0.005, Vector4d(0.0,0.0,1.0))); }

   gluLookAt(_eye.x,            _eye.y,            _eye.z,
             _center.x,         _center.y,         _center.z,
             _up.x,             _up.y,             _up.z);
}

void CamManager::onMouseHold(int x, int y) {
   Vector4d relativeMouse = Vector4d(x,y,0.0) - lastClickPos;
   Vector4d forward = (_center - _eye).Normalized();
   Vector4d right = (_up * forward).Normalized();
   Vector4d up = (right * forward).Normalized();

   if (keys[wheel]) {
      _center = c_center + right * relativeMouse.x;
      _eye = c_eye + right * relativeMouse.x;
      _center += up * -relativeMouse.y;
      _eye += up * -relativeMouse.y;
      //rotateCameraByEye(c_center, c_up, Matrix4d::Rotation(-relativeMouse.x, c_up));
      //rotateCameraByEye(_center, _up, Matrix4d::Rotation(relativeMouse.y, _up * (_center - _eye)));
   }
   else {
      relativeMouse.x = relativeMouse.x * DEGREES_TO_RADIANS / 5.0;
      relativeMouse.y = relativeMouse.y * DEGREES_TO_RADIANS / 5.0;
      rotateCameraByCenter(c_eye, c_up, Matrix4d::Rotation(relativeMouse.x, c_up));
      rotateCameraByCenter(_eye, _up, Matrix4d::Rotation(-relativeMouse.y, _up * (_center - _eye)));
   }
}

void CamManager::onMouseClick(int button, int x, int y) {
   if (button == 1) keys[wheel] = true;
   else keys[wheel] = false;

  lastClickPos = (Vector4d((double)x,(double)y,0.0));
  c_eye = _eye;
  c_center = _center;
  c_up = _up;

  Vector4d forward = (_eye-_center).Normalized();
  
  if (button==3) {
     _eye -= forward*20.0;
     _center -= forward*20.0;
  } else if (button==4) {
     _eye += forward*20.0;
     _center += forward*20.0;
  }
}

