#pragma once


#include "accumulatorball_t.h"
#include "octree_t.h"
#include "kernel_t.h"
#include "bin_t.h"


void voting(octree_t &root, accumulatorball_t &accum, std::vector<bin_t> &used_bins, double max_point_distance);