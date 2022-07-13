#include "Matrix4d.h"
#include "Vector4d.h"
#include <stdio.h>
#include <cmath>

void Matrix4d::swap(double& f1, double& f2) {
   double k = f1;
   f1 = f2;
   f2 = k;
}


Matrix4d::Matrix4d() {
   *this = Zeros();
}


Matrix4d::Matrix4d(const Matrix4d& m) {
   int i;
   for (i=0; i<16; i++)
      data[i] = m.data[i];
}


Matrix4d::Matrix4d(double* f) {
   int i;
   for (i=0; i<16; i++)
      data[i] = f[i];
}


Matrix4d::~Matrix4d() {
   /* none */ ;
}


Matrix4d& Matrix4d::operator =  (const Matrix4d& m) {
   int i;
   for (i=0; i<16; i++)
      data[i] = m.data[i];
   return *this;
}


Matrix4d& Matrix4d::operator += (const Matrix4d& m) {
   int i;
   for (i=0; i<16; i++)
      data[i] += m.data[i];
   return *this;
}



Matrix4d& Matrix4d::operator -= (const Matrix4d& m) {
   int i;
   for (i=0; i<16; i++)
      data[i] -= m.data[i];
   return *this;
}


Matrix4d& Matrix4d::operator *= (const Matrix4d& m) {
   Matrix4d t(*this);
   *this = t * m;
   return *this;
}


Matrix4d& Matrix4d::operator += (double val) {
   int i;
   for (i=0; i<16; i++)
      data[i] += val;
   return *this;
}


Matrix4d& Matrix4d::operator -= (double val) {
   int i;
   for (i=0; i<16; i++)
      data[i] -= val;
   return *this;
}


Matrix4d& Matrix4d::operator *= (double val) {
   int i;
   for (i=0; i<16; i++)
      data[i] *= val;
   return *this;
}


Matrix4d& Matrix4d::operator /= (double val) {
   int i;
   for (i=0; i<16; i++)
      data[i] /= val;
   return *this;
}



const Matrix4d Matrix4d::operator + (const Matrix4d& m) const {
   Matrix4d t = Matrix4d(*this);
   return t += m;
}


const Matrix4d Matrix4d::operator - (const Matrix4d& m) const {
   Matrix4d t = Matrix4d(*this);
   return t -= m;
}


const Matrix4d Matrix4d::operator * (const Matrix4d& m) const {
   Matrix4d t = Zeros();

   int i, j, k;
   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         for (k=0; k<4; k++)
            t.data[i*4+j] += data[i*4+k] * m.data[k*4+j];
   return t;
}


const Matrix4d Matrix4d::operator + (double val) const {
   Matrix4d t = Matrix4d(*this);
   return t += val;
}


const Matrix4d Matrix4d::operator - (double val) const {
   Matrix4d t = Matrix4d(*this);
   return t -= val;
}


const Matrix4d Matrix4d::operator * (double val) const {
   Matrix4d t = Matrix4d(*this);
   return t *= val;
}


const Matrix4d Matrix4d::operator / (double val) const {
   Matrix4d t = Matrix4d(*this);
   return t /= val;
}



double Matrix4d::operator()(int index1, int index2) {
   return data[index1*4+index2];
}


bool Matrix4d::operator == (const Matrix4d& m) const {
   int i;
   for (i=0; i<16; i++)
      if (data[i] != m.data[i]) return false;
   return true;
}


bool Matrix4d::operator != (const Matrix4d& m) const {
   int i;
   for (i=0; i<16; i++)
      if (data[i] != m.data[i]) return true;
   return false;
}


const Matrix4d& Matrix4d::Identity() {
   static double I[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f};
   static Matrix4d mI = Matrix4d(I);
   return mI;
}


const Matrix4d& Matrix4d::Zeros() {
   static double Z[16] = {0.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 0.0f};
   static Matrix4d mZ = Matrix4d(Z);
   return mZ;
}


void Matrix4d::Invert() {
   double det = Det();
   double detij;
   int i, j;
   Matrix4d mInverse = Zeros();

   det = 1.0f / det;
   for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
         detij = DetIJ(data, j, i);
         mInverse.data[i*4+j] = ((i+j) & 0x1) ? (-detij * det) : (detij *det);
      }
   }
   *this = mInverse;
}


void Matrix4d::Transpose() {
   swap(data[1],  data[4]);
   swap(data[2],  data[8]);
   swap(data[3],  data[12]);
   swap(data[6],  data[9]);
   swap(data[7],  data[13]);
   swap(data[11], data[14]);
}


double Matrix4d::DetIJ(double* m, int i, int j) {
   int x, y, ii, jj;
   double ret, mat[3][3];

   x = 0;
   for (ii = 0; ii < 4; ii++)
   {
      if (ii == i) continue;
      y = 0;
      for (jj = 0; jj < 4; jj++)
      {
         if (jj == j) continue;
         mat[x][y] = m[ii*4+jj];
         y++;
      }
      x++;
   }

   ret =  mat[0][0]*(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2]);
   ret -= mat[0][1]*(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2]);
   ret += mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);

   return ret;
}


double Matrix4d::Det() {
	double det = 0.0f;
	int i;
	for (i = 0; i < 4; i++) {
		det += (i & 0x1) ? (-data[i] * DetIJ(data, 0, i)) : (data[i] * DetIJ(data, 0,i));
	}
	return det;
}



const Matrix4d Matrix4d::Translation(double x, double y, double z) {
   Matrix4d t = Identity();
   t.data[12] = x;
   t.data[13] = y;
   t.data[14] = z;
   return t;
}

const Matrix4d Matrix4d::Translation(const Vector4d& v) {
   Matrix4d t = Identity();
   t.data[3] = v.x;
   t.data[7] = v.y;
   t.data[11] = v.z;
   return t;
}



const Matrix4d Matrix4d::Scaling(double x, double y, double z) {
   Matrix4d t = Identity();
   t.data[0] = x;
   t.data[5] = y;
   t.data[10] = z;
   return t;
}


const Matrix4d Matrix4d::Rotation(double angle, double x, double y, double z) {
   double vecLength, sinSave, cosSave, oneMinusCos;
   double xx, yy, zz, xy, yz, zx, xs, ys, zs;
   Matrix4d t = Zeros();

   // Scale vector
   vecLength = sqrt(x*x + y*y + z*z);

   // Rotation matrix is normalized
   x /= vecLength;
   y /= vecLength;
   z /= vecLength;

   sinSave = sin(angle);
   cosSave = cos(angle);
   oneMinusCos = 1.0f - cosSave;

   xx = x * x;
   yy = y * y;
   zz = z * z;
   xy = x * y;
   yz = y * z;
   zx = z * x;
   xs = x * sinSave;
   ys = y * sinSave;
   zs = z * sinSave;
    
   t.data[0] = (oneMinusCos * xx) + cosSave;  t.data[1] = (oneMinusCos * xy) + zs;        t.data[2] = (oneMinusCos * zx) - ys;         t.data[3] = 0.0f;
   t.data[4] = (oneMinusCos * xy) - zs;       t.data[5] = (oneMinusCos * yy) + cosSave;   t.data[6] = (oneMinusCos * yz) + xs;         t.data[7] = 0.0f;
   t.data[8] = (oneMinusCos * zx) + ys;       t.data[9] = (oneMinusCos * yz) - xs;        t.data[10] = (oneMinusCos * zz) + cosSave;   t.data[11] = 0.0f;
   t.data[12] = 0.0f;                         t.data[13] = 0.0f;                          t.data[14] = 0.0f;                           t.data[15] = 1.0f;


   return t;

}

const Matrix4d Matrix4d::RotationQuaternion(double angle, double x, double y, double z) {
   double q0, q1, q2, q3, q12, q22, q32;
   Vector4d q;
   
   double cosHalfAngle = cos(angle/2.0);
   double sinHalfAngle = sin(angle/2.0);

   Vector4d u(x,y,z);
   u.Normalize();

   q0 = cosHalfAngle;
   q = u * sinHalfAngle;

   q1 = q[0];
   q2 = q[1];
   q3 = q[2];
   q12 = q1*q1;
   q22 = q2*q2;
   q32 = q3*q3;

   Matrix4d t;
 
   t.data[0] =  1.0 - 2.0 * (q22 + q32); t.data[1]  = 2.0 * (q1*q2 - q0*q3);   t.data[2]  = 2.0 * (q1*q3 + q0*q2);   t.data[3]  = 0.0;
   t.data[4] =  2.0 * (q1*q2 + q0*q3);   t.data[5]  = 1.0 - 2.0 * (q12 + q32); t.data[6]  = 2.0 * (q2*q3 - q0*q1);   t.data[7]  = 0.0;
   t.data[8] =  2.0 * (q1*q3 - q0*q2);   t.data[9]  = 2.0 * (q2*q3 + q0*q1);   t.data[10] = 1.0 - 2.0 * (q12 + q22); t.data[11] = 0.0;
   t.data[12] = 0.0;                     t.data[13] = 0.0;                     t.data[14] = 0.0;                     t.data[15] = 1.0;
  
   return t;

}


const Matrix4d Matrix4d::Rotation(double angle, const Vector4d& v) {
   return Rotation(angle,v.x,v.y,v.z);
}
const Matrix4d Matrix4d::RotationQuaternion(double angle, const Vector4d& v) {
   return RotationQuaternion(angle,v.x,v.y,v.z);
}

const Matrix4d Matrix4d::RotationX(double angle) {
   Matrix4d t = Zeros();

   double sina = sin(angle);
   double cosa = cos(angle);

   t.data[0] = 1.0f;     t.data[1] = 0.0f;      t.data[2] = 0.0f;      t.data[3] = 0.0f;
   t.data[4] = 0.0f;     t.data[5] = cosa;      t.data[6] = -sina;     t.data[7] = 0.0f;
   t.data[8] = 0.0f;     t.data[9] = sina;      t.data[10] = cosa;     t.data[11] = 0.0f;
   t.data[12] = 0.0f;    t.data[13] = 0.0f;     t.data[14] = 0.0f;     t.data[15] = 1.0f;


   return t;
}


const Matrix4d Matrix4d::RotationY(double angle) {
   Matrix4d t = Zeros();

   double sina = sin(angle);
   double cosa = cos(angle);


   t.data[0] = cosa;     t.data[1] = 0.0f;     t.data[2] = sina;     t.data[3] = 0.0f;
   t.data[4] = 0.0;      t.data[5] = 1.0f;     t.data[6] = 0.0f;     t.data[7] = 0.0f;
   t.data[8] = -sina;    t.data[9] = 0.0f;     t.data[10] = cosa;    t.data[11] = 0.0f;
   t.data[12] = 0.0f;    t.data[13] = 0.0f;    t.data[14] = 0.0f;    t.data[15] = 1.0f;

 
   return t;
}


const Matrix4d Matrix4d::RotationZ(double angle) {
   Matrix4d t = Zeros();

   double sina = sin(angle);
   double cosa = cos(angle);

   t.data[0] = cosa;    t.data[1] = -sina;   t.data[2] = 0.0f;    t.data[3] = 0.0f;
   t.data[4] = sina;    t.data[5] = cosa;    t.data[6] = 0.0f;    t.data[7] = 0.0f;
   t.data[8] = 0.0f;    t.data[9] = 0.0f;    t.data[10] = 1.0f;   t.data[11] = 0.0f;
   t.data[12] = 0.0f;   t.data[13] = 0.0f;   t.data[14] = 0.0f;   t.data[15] = 1.0f;

   return t;
}


const Vector4d Matrix4d::operator * (const Vector4d& rhs) const {

   return Vector4d(
      data[0] * rhs.data[0] + data[1] * rhs.data[1] + data[2] * rhs.data[2] + data[3] * rhs.data[3],
      data[4] * rhs.data[0] + data[5] * rhs.data[1] + data[6] * rhs.data[2] + data[7] * rhs.data[3],
      data[8] * rhs.data[0] + data[9] * rhs.data[1] + data[10] * rhs.data[2] + data[11] * rhs.data[3],
      data[12] * rhs.data[0] + data[13] * rhs.data[1] + data[14] * rhs.data[2] + data[15] * rhs.data[3]
   );
}



const Matrix4d Matrix4d::Perspective(double r) {
	Matrix4d t = Matrix4d::Identity();
	t.data[10] = 0.0f;
	t.data[14] = -1.0f / r;
	return t;
}
