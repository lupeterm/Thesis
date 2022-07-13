#pragma once

#include "SwissArmyKnife/Mathematic.h"
#include "SwissArmyKnife/Vector4d.h"
#include "dlib/matrix.h"
#include "settings.h"

#include <omp.h>

class octree_t
{
public:
   octree_t();

   void subdivide(hough_settings &settings);
   void least_variance_direction(void);
   void remove_outliers(void);
   void print_points(void);
   void show(bool type, int height);
   void show(bool type);
   void clear(void);
   void get_nodes(std::vector<octree_t *> &nodes);
   double distance2plane(Vector4d &point);
   dlib::matrix<double, 3, 3> fast_covariance_matrix(void);

   dlib::matrix<double, 3, 3> m_covariance;
   std::vector<Vector4d> m_points, m_colors;
   std::vector<int> m_indexes;

   octree_t *m_children, *m_root;
   Vector4d normal1, normal2, normal3, m_middle, m_centroid, color;
   double variance1, variance2, variance3, m_size, representativeness;
   short m_level;
   bool coplanar;
   int votes;
};
