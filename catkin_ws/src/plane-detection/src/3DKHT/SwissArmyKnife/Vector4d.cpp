#include "Vector4d.h"
#include "Matrix4d.h"
#include <cmath>

Vector4d::Vector4d(double _x, double _y, double _z, double _w) {
   data[0] = _x;
   data[1] = _y;
   data[2] = _z;
   data[3] = _w;
}

Vector4d::Vector4d(const Vector4d& v) {
   x = v.x;
   y = v.y;
   z = v.z;
   w = v.w;
}

Vector4d::Vector4d(const double* v) {
   data[0] = v[0];
   data[1] = v[1];
   data[2] = v[2];
   data[3] = v[3];
}

Vector4d::~Vector4d() {}

Vector4d& Vector4d::operator =  (const Vector4d& v) {
   x = v.x;
   y = v.y;
   z = v.z;
   w = v.w;
   return *this;
}

Vector4d& Vector4d::operator += (const Vector4d& v) {
   x += v.x;
   y += v.y;
   z += v.z;
   return *this;
}

Vector4d& Vector4d::operator -= (const Vector4d& v) {
   x -= v.x;
   y -= v.y;
   z -= v.z;
   return *this;
}

Vector4d& Vector4d::operator += (double val) {
   x += val;
   y += val;
   z += val;
   return *this;
}

Vector4d& Vector4d::operator -= (double val) {
   x -= val;
   y -= val;
   z -= val;
   return *this;
}

Vector4d& Vector4d::operator *= (double val) {
   x *= val;
   y *= val;
   z *= val;
   return *this;
}

Vector4d& Vector4d::operator *= (const Vector4d& v) {
   x *= v.x;
   y *= v.y;
   z *= v.z;
   return *this;
}


Vector4d& Vector4d::operator /= (double val) {
   x /= val;
   y /= val;
   z /= val;
   return *this;
}

Vector4d& Vector4d::operator /= (const Vector4d& v) {
   x /= v.x;
   y /= v.y;
   z /= v.z;
   return *this;
}


const Vector4d Vector4d::operator + (const Vector4d& v) const {
   Vector4d t(*this);
   return (t += v);
}


const Vector4d Vector4d::operator - (const Vector4d& v) const {
   Vector4d t(*this);
   return (t -= v);
}


const double Vector4d::operator & (const Vector4d& v) const { // escalar
   return x*v.x + y*v.y + z*v.z;
}


const Vector4d Vector4d::operator * (const Matrix4d& m) const {
   Vector4d vec;
   vec.data[0] = data[0] * m.data[0] + data[1] * m.data[4] + data[2] * m.data[8] + data[3] * m.data[12];
   vec.data[1] = data[0] * m.data[1] + data[1] * m.data[5] + data[2] * m.data[9] + data[3] * m.data[13];
   vec.data[2] = data[0] * m.data[2] + data[1] * m.data[6] + data[2] * m.data[10] + data[3] * m.data[14];
   vec.data[3] = data[0] * m.data[3] + data[1] * m.data[7] + data[2] * m.data[11] + data[3] * m.data[15];
   return vec;
}


const Vector4d Vector4d::operator * (const Vector4d& v) const { // vetorial
   return Vector4d(y * v.z - z * v.y,
                   z * v.x - x * v.z,
                   x * v.y - y * v.x,
                   1);
}

const Vector4d Vector4d::operator + (double val) const {
   Vector4d t(*this);
   return (t += val);
}


const Vector4d Vector4d::operator - (double val) const {
   Vector4d t(*this);
   return (t -= val);
}


const Vector4d Vector4d::operator * (double val) const {
   Vector4d t(*this);
   return (t *= val);
}
/*
const Vector4d Vector4d::operator * (double s, const Vector4d &v )
{
   return Vector4d(v * s);
}
*/

const Vector4d Vector4d::operator / (double val) const {
   Vector4d t(*this);
   return (t /= val);
}


bool Vector4d::operator == (const Vector4d& v) const {
   return (bool)(x == v.x && y == v.y && z == v.z && w == v.w);
}


bool Vector4d::operator != (const Vector4d& v) const {
   return (bool)(x != v.x || y != v.y || z != v.z || w != v.w);
}


void Vector4d::Normalize() {
   double lenght = GetLength();
   x /= lenght;
   y /= lenght;
   z /= lenght;
}


double Vector4d::GetLength() const {
   return sqrt(x*x + y*y + z*z);
}


const Vector4d Vector4d::operator - () const {
   return Vector4d(-data[0], -data[1], -data[2], -data[3]);
}


const Vector4d Vector4d::Normalized() const {
   return (*this / GetLength());
}

double Vector4d::Distance(const Vector4d& v) const {
   return sqrt(pow(x-v.x,2)+pow(y-v.y,2)+pow(z-v.z,2));
}

const Vector4d Vector4d::Harmonized(void) const {
   Vector4d v = *this / data[3];
   v.w = 1.0;
   return v;
}

void Vector4d::Harmonize(void)  {
   *this /= data[3];
   this->w = 1.0;
}

Vector4d Vector4d::Zero()   {return Vector4d(0.0,0.0,0.0,1.0);}
Vector4d Vector4d::Black()  {return Vector4d(0.0,0.0,0.0,1.0);}
Vector4d Vector4d::White()  {return Vector4d(1.0,1.0,1.0,1.0);}
Vector4d Vector4d::Red()    {return Vector4d(1.0,0.0,0.0,1.0);}
Vector4d Vector4d::Orange() {return Vector4d(1.0,0.5,0.0,1.0);}
Vector4d Vector4d::Yellow() {return Vector4d(1.0,1.0,0.0,1.0);}
Vector4d Vector4d::Green()  {return Vector4d(0.0,1.0,0.0,1.0);}
Vector4d Vector4d::Cyan()   {return Vector4d(0.0,1.0,1.0,1.0);}
Vector4d Vector4d::Blue()   {return Vector4d(0.0,0.0,1.0,1.0);}
Vector4d Vector4d::Violet() {return Vector4d(1.0,0.0,1.0,1.0);}



