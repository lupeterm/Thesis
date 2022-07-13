#pragma once

#include <iostream>
#include <fstream>

#include "octree_t.h"

void read_file(hough_settings &settings, octree_t &node)
{

   std::ifstream file;
   bool reading = false;
   int point_num = 0;

   double mix, miy, miz, max, may, maz;

   file.open(settings.file + settings.extension);

   if (file.is_open())
   {
      std::string currentLine;
      while (getline(file, currentLine))
      {
         Vector4d point, color(160, 160, 160);
         sscanf(currentLine.c_str(), "%lf %lf %lf %lf %lf %lf", &point.x, &point.y, &point.z, &color.x, &color.y, &color.z);
         node.m_points.push_back(point);
         node.m_centroid += point;
         node.m_indexes.push_back(point_num++);
         node.m_colors.push_back(color / 255.0);

         mix = min(mix, point.x);
         miy = min(miy, point.y);
         miz = min(miz, point.z);
         max = std::max(max, point.x);
         may = std::max(may, point.y);
         maz = std::max(maz, point.z);
      }
   }
   file.close();
}

void load_point_cloud(hough_settings &settings, octree_t &node)
{

   std::cout << "Loading Point Cloud...          " << std::endl;

   std::cout << "File: " << settings.file << "\"" << std::endl;
   auto start = std::chrono::high_resolution_clock::now();

   node.m_middle = Vector4d(0, 0, 0);
   node.m_level = 0;
   node.m_root = &node;

   read_file(settings, node);

   std::cout << "Size: " << node.m_points.size() << std::endl
             << "Loaded in: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0 << " seconds." << std::endl
             << std::endl;

   settings.s_ms = settings.s_ps * node.m_points.size();
}