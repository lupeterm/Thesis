#pragma once

#include "accum_ball_cell_t.h"
#include "SwissArmyKnife/Mathematic.h"
#include "bin_t.h"

#include <cstring>
#include <vector>

class accumulatorball_t
{
public:
   accumulatorball_t(const double max_distance, const int rho_num, const int phi_num);
   ~accumulatorball_t();

   inline void initialize(double theta_index, int phi_index)
   {
      int t = get_theta_index(theta_index, phi_index);
      if (m_data[phi_index][t] == NULL)
      {
         m_data[phi_index][t] = new accum_ball_cell_t(m_rho_length);
      }
   }

   inline bool visited_neighbour(const double theta_index, const short phi_index, const short rho_index)
   {
      std::vector<accum_cell_t *> neighbors = get_neighbors(theta_index, phi_index, rho_index, 27);
      for (accum_cell_t *cell : neighbors)
      {
         if (cell->visited)
            return true;
      }
      return false;
   }

   inline std::vector<octree_t *> convolution_nodes(const double theta_index, const short phi_index, const short rho_index)
   {

      std::vector<octree_t *> nodes;
      std::vector<accum_cell_t *> neighbors = get_neighbors(theta_index, phi_index, rho_index, 27);
      for (accum_cell_t *cell : neighbors)
      {
         for (octree_t *n : cell->ref_node)
         {
            std::vector<octree_t *>::iterator it = find(nodes.begin(), nodes.end(), n);
            if (it == nodes.end())
            {
               nodes.push_back(n);
            }
         }
      }
      return nodes;
   }

   inline ACCUM_BIN_TYPE convolution_value(const double theta_index, const int phi_index, const int rho_index)
   {
      ACCUM_BIN_TYPE acc_value = 0.0;
      std::vector<accum_cell_t *> neighbors = get_neighbors(theta_index, phi_index, rho_index, 5);

      acc_value = neighbors[0]->bin * 0.2;
      for (unsigned int i = 1; i < neighbors.size(); i++)
      {
         acc_value += neighbors[i]->bin * 0.1333;
      }

      return acc_value;
   }

   inline double normalization_factor(int phi_index)
   {
      return 360.0 / (double)m_data[phi_index].size();
   }

   inline void set_visited(const double theta_index, const short phi_index, const short rho_index)
   {
      std::vector<accum_cell_t *> neighbors = get_neighbors(theta_index, phi_index, rho_index, 27);
      for (accum_cell_t *cell : neighbors)
      {
         cell->visited = true;
      }
   }

   inline double delta_theta(const double &phi_index)
   {
      return m_theta_max / (double)(m_data[phi_index].size());
   }

   inline double delta_theta_index(const double &phi_index)
   {
      return 1.0 / (double)(m_data[phi_index].size());
   }

   inline void process_theta(double &theta_index)
   {
      theta_index = (theta_index + 1.0) - (int)(theta_index + 1.0);
   }

   inline bool process_phi(double &theta_index, int &phi_index)
   {
      process_theta(theta_index);

      if (phi_index < 0)
      {
         phi_index = abs(phi_index);
         theta_index = (theta_index + 0.5) - (int)((theta_index + 0.5));
         return true;
      }
      else if (phi_index > m_phi_length)
      {
         phi_index = m_phi_length + m_phi_length - phi_index;
         theta_index = (theta_index + 0.5) - (int)((theta_index + 0.5));
         return true;
      }
      return false;
   }

   inline bool process_rho(double &theta_index, int &phi_index, int &rho_index)
   {
      if (rho_index < 0)
      {
         rho_index = abs(rho_index);
         phi_index = m_phi_length - phi_index;
         theta_index = (theta_index + 0.5) - (int)((theta_index + 0.5));
         return true;
      }
      else if (rho_index >= m_rho_length)
      {
         return false;
      }
      return true;
   }

   inline std::vector<accum_cell_t *> get_neighbors(const double theta_index, const short phi_index, const short rho_index, const int neighborhood_size)
   {
      std::vector<accum_cell_t *> result;
      result.reserve(neighborhood_size);

      int p, r;
      double t, theta;

      if (phi_index == 0 || phi_index == m_phi_length)
         theta = 0.5;
      else
         theta = theta_index;

      //                                 center[1]   /  direct-linked[7]  /         /          semi-direct-linked[19]          /      /     diagonal-linked[27]    /
      static const short offset_x[27] = {
          0,
          0,
          0,
          0,
          0,
          +1,
          -1,
          +1,
          -1,
          +1,
          -1,
          +1,
          -1,
          -1,
          +1,
          0,
          0,
          0,
          0,
          +1,
          +1,
          +1,
          -1,
          +1,
          -1,
          -1,
          -1,
      };
      static const short offset_y[27] = {
          0,
          +1,
          -1,
          0,
          0,
          0,
          0,
          +1,
          -1,
          -1,
          +1,
          0,
          0,
          0,
          0,
          +1,
          -1,
          -1,
          +1,
          +1,
          +1,
          -1,
          +1,
          -1,
          -1,
          +1,
          -1,
      };
      static const short offset_z[27] = {
          0,
          0,
          0,
          +1,
          -1,
          0,
          0,
          0,
          0,
          0,
          0,
          +1,
          -1,
          +1,
          -1,
          +1,
          -1,
          +1,
          -1,
          +1,
          -1,
          +1,
          +1,
          -1,
          +1,
          -1,
          -1,
      };

      for (short i = 0; i != neighborhood_size; ++i)
      {
         t = theta;
         p = phi_index + offset_y[i];
         r = rho_index + offset_z[i];
         process_phi(t, p);
         t = fix_theta(t, p);
         t += delta_theta_index(p) * (double)(offset_x[i]);

         if (process_limits(t, p, r) == false)
            continue;

         accum_cell_t *cell = &at(t, p, r);
         std::vector<accum_cell_t *>::iterator it = find(result.begin(), result.end(), cell);

         if (it == result.end())
         {
            result.push_back(cell);
         }
      }
      return result;
   }

   inline bool process_limits(double &theta_index, int &phi_index, int &rho_index)
   {
      process_phi(theta_index, phi_index);
      return process_rho(theta_index, phi_index, rho_index);
   }

   inline accum_cell_t &at(const double theta, const short phi, const short rho)
   {
      int t = get_theta_index(theta, phi);
      if (m_data[phi][t] == NULL)
         m_data[phi][t] = new accum_ball_cell_t(m_rho_length);
      return m_data[phi][t]->bins[rho];
   }

   inline int get_theta_index(const double &theta, const int &phi_index)
   {
      return static_cast<int>(round(theta * (double)(m_data[phi_index].size()))) % m_data[phi_index].size();
   }

   inline void get_index(const double &theta, const double &phi, const double &rho, double &theta_index, int &phi_index, int &rho_index)
   {
      theta_index = theta / PI2 + 0.5;
      phi_index = round(phi / m_delta_angle);
      rho_index = round(rho / m_delta_rho);
   }

   inline void get_values(double &theta, double &phi, double &rho, const double &theta_index, const int &phi_index, const int &rho_index)
   {
      theta = (theta_index - 0.5) * PI2;
      phi = (double)(phi_index)*m_delta_angle;
      rho = (double)(rho_index)*m_delta_rho;
   }

   inline void spherical_to_cartesian(Vector4d &normal, const double &theta, const double &phi, const double &rho)
   {
      normal.x = sin(phi) * cos(theta) * rho;
      normal.y = sin(phi) * sin(theta) * rho;
      normal.z = cos(phi) * rho;
   }

   inline double fix_theta(double theta, const int phi)
   {
      double p_size = m_data[phi].size();
      int t = round((theta)*p_size);
      return (t == 1) ? (0.0) : t / p_size;
   }

   int neighbors_size;

   double m_theta_max;
   double m_phi_max;

   short m_phi_length; // Accumulator size (phi dimension)
   short m_phi_length_half;
   short m_rho_length;   // Accumulator size (rho dimension)
   short m_theta_length; // Accumulator size (theta dimension)

   double m_delta_rho;   // Discretization step to the distance (rho)
   double m_delta_angle; // Discretization step to the angles (theta & phi)

   std::vector<std::vector<accum_ball_cell_t *>> m_data;
};