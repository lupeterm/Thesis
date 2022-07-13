#include "hough.h"
#include "peak_detection.h"
#include "voting.h"
#include "Timer.h"
#include "octree_t.h"
#include "plane_t.h"
#include "settings.h"
#include <vector>
#include <iostream>
#include <chrono>

#include "SwissArmyKnife/Mathematic.h"
#include "accumulatorball_t.h"

accumulatorball_t *kht3d(std::vector<plane_t> &planes, octree_t &father, hough_settings &settings) // const double max_distance, const double distance_discretization, const int phi_cells_length )
{

#ifndef realtime

   std::cout << "3-D KHT starting..." << std::endl;

   auto s_start = std::chrono::high_resolution_clock::now();
   std::cout << "Subdividing...                  ";
#endif

   // Subdividing Procedure
   father.subdivide(settings);

#ifndef realtime
   auto s_end = std::chrono::high_resolution_clock::now();
   auto s_elasped = std::chrono::duration_cast<std::chrono::microseconds>(s_end - s_start).count() / 1000000.0;
   std::cout << "Done [" << s_elasped << "s]" << std::endl;
#endif

#ifndef realtime
   auto a_start = std::chrono::high_resolution_clock::now();
#endif

   // Initializes the Accumulator
   accumulatorball_t *accum = new accumulatorball_t(settings.max_point_distance, settings.rho_num, settings.phi_num);

#ifndef realtime
   auto v_start = std::chrono::high_resolution_clock::now();
#endif

   // Voting Procedure
   std::vector<bin_t> used_bins;
   voting(father, *accum, used_bins, settings.max_point_distance);

#ifndef realtime
   auto v_end = std::chrono::high_resolution_clock::now();
   auto v_elasped = std::chrono::duration_cast<std::chrono::microseconds>(v_end - v_start).count() / 1000000.0;
   std::cout << "Voting...                       ";
   std::cout << "Done [" << v_elasped << "s]" << std::endl;
   auto d_start = std::chrono::high_resolution_clock::now();
   std::cout << "Detecting Peaks....             ";
#endif

   // Peak Detection Procedure
   peak_detection(planes, *accum, used_bins);

#ifndef realtime
   auto d_end = std::chrono::high_resolution_clock::now();
   auto d_elasped = std::chrono::duration_cast<std::chrono::microseconds>(d_end - d_start).count() / 1000000.0;
   std::cout << "Done [" << d_elasped << "s]" << std::endl;

#endif

   for (plane_t &p : planes)
   {
      accum->at(p.ti, p.pi, p.ri).peak = true;
   }

   // Sorting planes by representativeness
   for (unsigned int i = 0; i < planes.size(); i++)
   {
      planes[i].representativeness = 0;
      for (unsigned int j = 0; j < planes[i].nodes.size(); j++)
         planes[i].representativeness += planes[i].nodes[j]->representativeness;
   }
   std::sort(planes.begin(), planes.end());

#ifdef debugplanes

   // Coloring planes and points
   for (unsigned int i = 0; i < planes.size(); i++)
   {
      Vector4d cor;
      switch (i % 6)
      {
      case 0:
         cor = Vector4d((int)(255 / (int)(i / 6 + 1)), 0, 0) / 255.0;
         break;
      case 1:
         cor = Vector4d(0, (int)(255 / (int)(i / 6 + 1)), 0) / 255.0;
         break;
      case 2:
         cor = Vector4d(0, 0, (int)(255 / (int)(i / 6 + 1))) / 255.0;
         break;
      case 3:
         cor = Vector4d(0, (int)(255 / (int)(i / 6 + 1)), (int)(255 / (int)(i / 6 + 1))) / 255.0;
         break;
      case 4:
         cor = Vector4d((int)(255 / (int)(i / 6 + 1)), 0, (int)(255 / (int)(i / 6 + 1))) / 255.0;
         break;
      case 5:
         cor = Vector4d((int)(255 / (int)(i / 6 + 1)), (int)(255 / (int)(i / 6 + 1)), 0) / 255.0;
         break;
      }

      planes[i].m_color = cor;

      for (unsigned int j = 0; j < planes[i].nodes.size(); j++)
         planes[i].nodes[j]->color = cor;
   }

   for (unsigned int i = 0; i < father.m_points.size(); i++)
   {
      for (unsigned int p = 0; p < planes.size(); p++)
      {
         if (planes[p].distance2plane(father.m_points[i]) < settings.max_distance2plane)
         {
            father.m_colors[i] = planes[p].m_color;
            break;
         }
      }
   }
#endif

   used_bins.clear();

   return accum;
}
