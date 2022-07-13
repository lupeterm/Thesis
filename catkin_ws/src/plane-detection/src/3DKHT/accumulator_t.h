/*#pragma once

#include "accum_t.h"
#include "Mathematic.h"

#include <cstring>
#include <vector>
/*
#define DEBUG
//
class octree_t;

class accumulator_t : public accum_t {
public:

   accumulator_t(const double max_distance, const double distance_discretization, const double angle_in_degrees_discretization) : accum_t()
   {
      m_delta_rho = distance_discretization;
      m_rho_length = round(max_distance/m_delta_rho);
      
      m_delta_angle = angle_in_degrees_discretization * DEGREES_TO_RADIANS;
      m_theta_length = round( 2.0 * PI / m_delta_angle );
      m_phi_length = round(  PI / m_delta_angle );

      m_rho_length_half = m_rho_length/2;
      m_theta_length_half = m_theta_length/2;

      
      std::cout << "Accumulator discretization:" << std::endl;
      std::cout << "    Width: " << m_theta_length << std::endl;
      std::cout << "    Height: " << m_phi_length << std::endl;
      std::cout << "    Rho: " << m_rho_length << std::endl;
      

      clear();
   }

   ~accumulator_t()
   {
      delete [] data;
   }

   void clear() 
   {
      //data = new accum_cell[m_width*m_height*m_depth]();
      data = new accum_cell_t[m_theta_length*m_phi_length*m_rho_length];
   }

   inline bool visited_neighbour(const short theta_index, const short phi_index, const short rho_index) 
   {
      int t, p, r;
      static const short offset_x[27] = {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1};
      static const short offset_y[27] = {-1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0,  0,  0,  1,  1,  1};
      static const short offset_z[27] = {-1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1};

      for (short i=0; i!=27; ++i)
      {
         t = theta_index + offset_x[i];
         p = phi_index + offset_y[i];
         r = rho_index + offset_z[i];

         if (process_limits(t,p,r) == false) continue;

         if (data[t * m_phi_length*m_rho_length + p*m_rho_length + r].visited)
            return true;
      }
      return false;
   }
   
   inline bool process_limits(double &theta_index, int &phi_index, int &rho_index)
   {
      if (theta_index < 0) 
      {
         theta_index += m_theta_length;         
      } else if (theta_index >= m_theta_length)
      {
         theta_index -= m_theta_length;
      }
      if (phi_index < 0) {
         phi_index += m_phi_length;
         rho_index = ((rho_index - m_rho_length_half) * (-1)) + m_rho_length_half;
      }
      else if (phi_index >= m_phi_length) {
         phi_index -= m_phi_length;
         rho_index = ((rho_index - m_rho_length_half) * (-1)) + m_rho_length_half;
      }
      
      if (rho_index < 0 || rho_index >= m_rho_length) return false;
      return true;
   }


   // Computes the convolution of the given cell with a (discrete) 3x3 Gaussian kernel.
   inline int convolutionValue(const int theta_index, const int phi_index, const int rho_index)
   {
      int acc_value = 0, t, p, r;

      static const short offset_x[27] = {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1};
      static const short offset_y[27] = {-1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0,  0,  0,  1,  1,  1};
      static const short offset_z[27] = {-1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1};

      
      for (short i=0; i!=27; ++i)
      {
         t = theta_index + offset_x[i];
         p = phi_index + offset_y[i];
         r = rho_index + offset_z[i];
         
         if (process_limits(t,p,r) == false) continue;

         acc_value += (*this)(t,p,r).bin;
      }
      
      return acc_value;
   }

   inline void set_visited(const short theta_index, const short phi_index, const short rho_index) 
   {
      (*this)(theta_index,phi_index,rho_index).visited = true;
      //std::cout << theta_index << " " << phi_index << " " << rho_index << std::endl;
   }

   inline accum_cell_t &operator () (const short theta, const short phi , const short rho) 
   { 
      return this->data[theta * m_phi_length*m_rho_length + phi*m_rho_length + rho]; 
   }

   void get_index(const double &theta, const double &phi, const double &rho, double &theta_index, int &phi_index, int &rho_index) 
   {
      theta_index = round(theta / m_delta_angle) + m_theta_length_half;
      phi_index =   round(phi   / m_delta_angle);
      rho_index =   round(rho   / m_delta_rho)   + m_rho_length_half;
   }

   virtual void get_values(double &theta, double &phi, double &rho, const double &theta_index, const int &phi_index, const int &rho_index) 
   {
      theta = (double)(theta_index - m_theta_length_half) * m_delta_angle;
      phi =   (double)(phi_index                        ) * m_delta_angle;
      rho =   (double)(rho_index   - m_rho_length_half  ) * m_delta_rho;
   }
   
   inline bool process_phi(int &phi_index, int &rho_index) {}
   inline void process_theta(double &theta_index, int &phi_index) {}
   

};*/