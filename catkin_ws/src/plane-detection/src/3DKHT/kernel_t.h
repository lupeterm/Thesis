#pragma once

#include "octree_t.h"
#include <math.h>

#define square(var) (var*var)
static const double root22pi32 = 2.0*sqrt(2.0)*pow(PI,1.5);


class kernel_t
{
public:

   octree_t *node;

   double rho;
   double theta;
   double phi;

   double constant_normal, constant_binormal;
   double voting_limit;

   double theta_index;
   int phi_index;
   int rho_index;

   int votes;

   dlib::matrix<double,3,3> covariance_rpt_normal;
   dlib::matrix<double,3,3> covariance_rpt_inv_normal;

   inline void kernel_load_parameters(double max_point_distance) 
   {
      
      dlib::matrix<double,3,3> covariance_xyz = node->m_covariance;
      dlib::matrix<double,3,3> jacobian_normal;

      Vector4d n = node->normal1.Normalized();

      // Jacobian Matrix calculation
      double t=1.0;
      double EPS2 = 0.00001;
      Vector4d p = n * rho;
      double w = square(p.x) + square(p.y);
      double p2 = w + square(p.z);
      double sqrtW = sqrt(w);
      jacobian_normal(0,0) = n.x;                                                       jacobian_normal(0,1) = n.y;                                                       jacobian_normal(0,2) = n.z;
      jacobian_normal(1,0) = (sqrtW<EPS2)?(p.x * p.z)/EPS2:(p.x * p.z) / (sqrtW * p2);  jacobian_normal(1,1) = (sqrtW<EPS2)?(p.y * p.z)/EPS2:(p.y * p.z) / (sqrtW * p2);  jacobian_normal(1,2) = (p2<EPS2)?-sqrtW/EPS2:(-sqrtW / p2);
      jacobian_normal(2,0) = (w<EPS2)?-p.y/EPS2:-p.y / w;                               jacobian_normal(2,1) = (w<EPS2)?p.x/EPS2:p.x / w;                                 jacobian_normal(2,2) = 0.0;
      
      // Uncertainty propagation
      dlib::matrix<double,3,3> jacobian_transposed_normal = dlib::trans(jacobian_normal);
      covariance_rpt_normal = jacobian_normal * covariance_xyz * jacobian_transposed_normal;
   
      // Cluster representativeness
      covariance_rpt_normal(0,0) += NONZERO;
      covariance_rpt_inv_normal = dlib::inv(covariance_rpt_normal);
      constant_normal = root22pi32 * sqrt(abs(dlib::det(covariance_rpt_normal)));
      dlib::eigenvalue_decomposition< dlib::matrix<double,3,3> > eigenvalue_decomp(covariance_rpt_normal);
      dlib::matrix<double> eigenvalues_vector = eigenvalue_decomp.get_real_eigenvalues();
      dlib::matrix<double> eigenvectors_matrix = eigenvalue_decomp.get_pseudo_v();
      
      // Area importance (w_a) 
      double w_a = 0.75;
      // Number-of-points importance (w_d)
      double w_d = 1- w_a;
      node->representativeness = (double)node->m_size/(double)node->m_root->m_size * w_a + (double)node->m_indexes.size()/(double)node->m_root->m_points.size() * (w_d);

      // Sort eigenvalues
      int min_index = 0;
      if (eigenvalues_vector(min_index) > eigenvalues_vector(1))
         min_index = 1;
      if (eigenvalues_vector(min_index) > eigenvalues_vector(2))
         min_index = 2;

      // Voting limit calculation (g_min)
      double n_of_standard_variations = 2.0;
      double radius = sqrt( eigenvalues_vector(min_index) ) * n_of_standard_variations;
      voting_limit = trivariated_gaussian_dist_normal( eigenvectors_matrix(0, min_index) * radius, eigenvectors_matrix(1, min_index) * radius, eigenvectors_matrix(2, min_index) * radius);

   }

   // Sampling the Trivariate Gaussian Distribution 
   inline double trivariated_gaussian_dist_normal(const double rho, const double phi, const double theta) 
   {
      dlib::vector<double,3> displacement(rho,phi,theta);
      return (std::exp(-0.5 * (dlib::trans(displacement) * covariance_rpt_inv_normal * displacement))/constant_normal);
   }

};