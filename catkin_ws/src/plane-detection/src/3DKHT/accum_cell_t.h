#pragma once

#include "octree_t.h"


// A cell (θ, φ, ρ) of the accumulator which stores the votes
class accum_cell_t {
public:
   accum_cell_t()
   {
      last_casted_vote = 0.0;
      last_node_voted = NULL;
      visited = false;
      top = false;
      voted = false;
      peak = false;
      bin = 0.0;
   }

   ~accum_cell_t()
   {
      ref_node.clear();
   }

   inline bool verify_cell(octree_t * ref) 
   {
      return (last_node_voted==ref);
   }
   inline void apply_cell(octree_t * ref) {
      last_node_voted = ref;
   }

   inline void add_reference(octree_t * ref)
   {
      ref_node.push_back(ref);
   }

   std::vector<octree_t *> ref_node;

   octree_t *last_node_voted;
   bool peak, visited, voted, top;
   ACCUM_BIN_TYPE last_casted_vote, bin;

};