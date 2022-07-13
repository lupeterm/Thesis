#pragma once

#include <iostream>

class Matrix4d;

class Vector4d {
public:
   union
   {
      double data[4];
      struct
      {
         double x;
         double y;
         double z;
         double w;
      };
      struct
      {
         double r;
         double g;
         double b;
         double a;
      };
   };

   Vector4d(double x=0.0f, double y=0.0f, double z=0.0f, double w=1.0f);
   Vector4d(const double* v);
   Vector4d(const Vector4d& v);
   ~Vector4d();

   Vector4d& operator =  (const Vector4d& v);
   Vector4d& operator += (const Vector4d& v);
   Vector4d& operator -= (const Vector4d& v);
   Vector4d& operator *= (const Vector4d& v);
   Vector4d& operator /= (const Vector4d& v);
   Vector4d& operator += (double val);
   Vector4d& operator -= (double val);
   Vector4d& operator *= (double val);
   Vector4d& operator /= (double val);

   const Vector4d operator * (const Matrix4d& m) const;
   const Vector4d operator + (const Vector4d& v) const;
   const Vector4d operator - (const Vector4d& v) const;
   const double    operator & (const Vector4d& v) const;
   const Vector4d operator * (const Vector4d& v) const;
   const Vector4d operator + (double val) const;
   const Vector4d operator - (double val) const;
   const Vector4d operator * (double val) const;
   
   const Vector4d operator / (double val) const;
   const Vector4d operator - () const;
   
   inline friend const Vector4d operator * (double s, const Vector4d &v) {
      return Vector4d(v * s);
   }


   inline double& operator[](int index) { if ( index>=0 && index<=3 ) return data[index]; return data[0]; }
   bool operator == (const Vector4d& v) const;
   bool operator != (const Vector4d& v) const;

   operator double*() { return data; }

   void            Normalize(void);
   void            Harmonize(void);
   const Vector4d  Normalized(void) const;
   const Vector4d  Harmonized(void) const;
   double          GetLength() const;
   double          Distance(const Vector4d& v) const;
   
   static Vector4d Zero();
   static Vector4d Red();
   static Vector4d Green();
   static Vector4d Blue();
   static Vector4d White();
   static Vector4d Black();
   static Vector4d Cyan();
   static Vector4d Yellow();
   static Vector4d Magenta();
   static Vector4d Orange();
   static Vector4d Violet();
   

   friend std::ostream& operator<<(std::ostream& lhs, const Vector4d& rhs) {
      return (lhs << "X: " << rhs.x << " Y: " << rhs.y << " Z: " << rhs.z << " W: " << rhs.w);
   }


};



