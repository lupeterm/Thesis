#pragma once

#include <iostream>

class Vector4d;

class Matrix4d
{
   public:
      double data[16];

      Matrix4d();
      Matrix4d(const Matrix4d& m);
      Matrix4d(double* f);
      ~Matrix4d();

      Matrix4d& operator =  (const Matrix4d& m);
      Matrix4d& operator += (const Matrix4d& m);
      Matrix4d& operator -= (const Matrix4d& m);
      Matrix4d& operator *= (const Matrix4d& m);
      Matrix4d& operator += (double val);
      Matrix4d& operator -= (double val);
      Matrix4d& operator *= (double val);
      Matrix4d& operator /= (double val);

      const Matrix4d operator + (const Matrix4d& m) const;
      const Matrix4d operator - (const Matrix4d& m) const;
      const Matrix4d operator * (const Matrix4d& m) const;
      const Matrix4d operator + (double val) const;
      const Matrix4d operator - (double val) const;
      const Matrix4d operator * (double val) const;
      const Matrix4d operator / (double val) const;

      const Vector4d operator * (const Vector4d& m) const;

      double operator()(int index1, int index2);
      bool operator == (const Matrix4d& m) const;
      bool operator != (const Matrix4d& m) const;

      operator double*() { return data; }

      static const Matrix4d& Identity();
      static const Matrix4d& Zeros();
      void Invert();
      void Transpose();

      static const Matrix4d Translation(double x, double y, double z);
      static const Matrix4d Translation(const Vector4d& v);
      static const Matrix4d Scaling(double x, double y, double z);
      static const Matrix4d Rotation(double angle, double x, double y, double z);
      static const Matrix4d RotationQuaternion(double angle, double x, double y, double z);
      static const Matrix4d Rotation(double angle, const Vector4d& v);
      static const Matrix4d RotationQuaternion(double angle, const Vector4d& v);
      static const Matrix4d RotationX(double angle);
      static const Matrix4d RotationY(double angle);
      static const Matrix4d RotationZ(double angle);
      static const Matrix4d Perspective(double r);


      friend std::ostream& operator<<(std::ostream& lhs, const Matrix4d& rhs) {
         return (lhs << rhs.data[0] << "  "  << rhs.data[1]  << "  " << rhs.data[2]  << "  " << rhs.data[3]  << std::endl
                     << rhs.data[4] << "  "  << rhs.data[5]  << "  " << rhs.data[6]  << "  " << rhs.data[7]  << std::endl
                     << rhs.data[8] << "  "  << rhs.data[9]  << "  " << rhs.data[10] << "  " << rhs.data[11] << std::endl
                     << rhs.data[12] << "  " << rhs.data[13] << "  " << rhs.data[14] << "  " << rhs.data[15] << std::endl);
      }
      

      double Det();

   private:
      static double DetIJ(double* m, int i, int j);
      void swap(double& f1, double& f2);

};


