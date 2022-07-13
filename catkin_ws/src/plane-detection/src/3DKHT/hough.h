#pragma once

#include "plane_t.h"
#include "accumulatorball_t.h"

#include <vector>

class hough_settings;
class octree_t;

accumulatorball_t * kht3d( std::vector<plane_t> &planes, octree_t &father, hough_settings &settings);

