/*#pragma once

#include "Vector4d.h"
#include "Mathematic.h"
#include "accum_cell_t.h"
#include "accum_ball_cell_t.h"
#include "bin_t.h"
#include <vector>

class accum_t {
public:
   accum_t() {
      m_theta_max = PI2;
      m_phi_max = PI;
   }

   virtual bool visited_neighbour(const double theta_index, const short phi_index, const short rho_index) = 0;
   virtual int  convolution_value(const double theta_index, const int phi_index, const int rho_index) = 0;
   virtual void convolution_vote(const double theta_index, const int phi_index, const int rho_index, const int &votes, octree_t * node, std::vector<bin_t> &used_bins) = 0;
   virtual std::vector<octree_t *> convolution_nodes(const double theta_index, const short phi_index, const short rho_index) = 0;

   virtual void process_theta(double &theta_index) = 0;
   virtual bool process_phi(double &theta_index, int &phi_index) = 0;
   virtual bool process_rho(double &theta_index, int &phi_index, int &rho_index) = 0;
   virtual bool process_limits(double &theta_index, int &phi_index, int &rho_index) = 0;

   virtual void set_visited(const double theta_index, const short phi_index, const short rho_index) = 0;
   
   virtual void get_index(const double &theta, const double &phi, const double &rho, double &theta_index, int &phi_index, int &rho_index) = 0;
   virtual void get_values(double &theta, double &phi, double &rho, const double &theta_index, const int &phi_index, const int &rho_index) = 0;
   virtual int  get_theta_index(const double &theta, const int &phi_index) = 0;

   virtual double delta_theta(const double &phi_index) = 0;
   virtual double delta_theta_index(const double &phi_index) = 0;
   
   virtual void spherical_to_cartesian(Vector4d &normal, const double &theta, const double &phi, const double &rho) = 0;

   virtual accum_cell_t &operator () (const double theta, const short phi , const short rho) = 0;
   
   double m_theta_max;
   double m_phi_max;

   short m_phi_length;         // Accumulator size (phi dimension)
   short m_rho_length;         // Accumulator size (rho dimension)
   short m_theta_length;       // Accumulator size (theta dimension)
   //short m_rho_length_half;    // Half accumulator size (rho dimension)  
   //short m_theta_length_half;  // Half accumulator size (rho dimension)

   double m_delta_rho;         // Discretization step to the distance (rho)
   double m_delta_angle;       // Discretization step to the angles (theta & phi)

   std::vector< std::vector <accum_ball_cell_t> > m_data;
};

*/