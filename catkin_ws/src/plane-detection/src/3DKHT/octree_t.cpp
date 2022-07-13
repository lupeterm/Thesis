#include "plane_t.h"
#include "octree_t.h"

octree_t::octree_t()
{
   m_children = NULL;
   coplanar = false;
   m_centroid = Vector4d(0,0,0,0);
   color = Vector4d(0.5,0.5,0.5);
   variance1 = variance2 = variance3 = 0.0;
   votes= 0;
}


void octree_t::clear()
{
   if (m_children != NULL)
   {
      for (short i = 0; i < 8 ; i++)
      {
         m_children[i].m_indexes.clear();
         m_children[i].clear();
      }
      delete [] m_children;
      m_children = NULL;
   }
   m_children = NULL;
}

void octree_t::subdivide( hough_settings &settings )
{

   // s_ms verification
	if (m_indexes.size() < (unsigned int)settings.s_ms) return;

   // s_level verification
   if (m_level >= settings.s_level)
   {
      // principal component analysis
      least_variance_direction();

      // Planarity verification
      double thickness = variance1 / variance2;
      double isotropy  = variance2 / variance3;
      if (thickness < settings.max_thickness && isotropy > settings.min_isotropy) {

         // Refitting step
         remove_outliers();
         least_variance_direction();
         
         coplanar = true;
         return;
      }
   }

   m_children = new octree_t[8];
   double newsize = m_size/2.0;
   for (int i = 0; i < 8 ; i++) {
      m_children[i].m_size = newsize;
      m_children[i].m_level = m_level+1;
      m_children[i].m_root = m_root;
      m_children[i].m_indexes.reserve(m_indexes.size()/4);
   }

   double size4 = m_size/4.0;

   // Calculation of son nodes
   m_children[0].m_middle.x = m_middle.x - size4;
   m_children[1].m_middle.x = m_middle.x - size4;
   m_children[2].m_middle.x = m_middle.x - size4;
   m_children[3].m_middle.x = m_middle.x - size4;
   m_children[4].m_middle.x = m_middle.x + size4;
   m_children[5].m_middle.x = m_middle.x + size4;
   m_children[6].m_middle.x = m_middle.x + size4;
   m_children[7].m_middle.x = m_middle.x + size4;

   m_children[0].m_middle.y = m_middle.y - size4;
   m_children[1].m_middle.y = m_middle.y - size4;
   m_children[2].m_middle.y = m_middle.y + size4;
   m_children[3].m_middle.y = m_middle.y + size4;
   m_children[4].m_middle.y = m_middle.y - size4;
   m_children[5].m_middle.y = m_middle.y - size4;
   m_children[6].m_middle.y = m_middle.y + size4;
   m_children[7].m_middle.y = m_middle.y + size4;

   m_children[0].m_middle.z = m_middle.z - size4;
   m_children[1].m_middle.z = m_middle.z + size4;
   m_children[2].m_middle.z = m_middle.z - size4;
   m_children[3].m_middle.z = m_middle.z + size4;
   m_children[4].m_middle.z = m_middle.z - size4;
   m_children[5].m_middle.z = m_middle.z + size4;
   m_children[6].m_middle.z = m_middle.z - size4;
   m_children[7].m_middle.z = m_middle.z + size4;

   // putting points in its respective children
   for (unsigned int i = 0; i < m_indexes.size() ; i++)
   {
      unsigned int index = 0;
      if (m_root->m_points[m_indexes[i]].x > m_middle.x)
      {
         index+=4;
      }
      if (m_root->m_points[m_indexes[i]].y > m_middle.y)
      {
         index+=2;
      }
      if (m_root->m_points[m_indexes[i]].z > m_middle.z)
      {
         index+=1;
      }
      m_children[index].m_indexes.push_back(m_indexes[i]);
      // Calculating centroid distribution (divided by the number of points below)
      m_children[index].m_centroid += m_root->m_points[m_indexes[i]];
   }
   
#pragma omp parallel for 
   for (int i = 0; i < 8 ; i++) {
      m_children[i].m_centroid /= m_children[i].m_indexes.size();
      
      // Recursive subdivision 
      m_children[i].subdivide(settings);
   }
}

void octree_t::remove_outliers()
{
   Vector4d centroid;
   for (int i = m_indexes.size()-1; i >=0 ; i--)
   {
      if (distance2plane(m_root->m_points[m_indexes[i]]) > m_size/10.0) {
         m_indexes.erase(m_indexes.begin()+i);
      } else {
         centroid += m_root->m_points[m_indexes[i]];
      }
   }
   if (m_indexes.size() > 0)
      m_centroid = centroid/m_indexes.size();
}

dlib::matrix<double,3,3> octree_t::fast_covariance_matrix()
{
   unsigned int nverts = m_indexes.size();
   double nvertsd = (double)(nverts);
   dlib::matrix<double,3,3> covariance(3,3);

   
   covariance(0,0) = 0.0;
   for (size_t k = 0; k < nverts; k++)
      covariance(0,0) += (m_root->m_points[m_indexes[k]][0] - m_centroid[0]) * (m_root->m_points[m_indexes[k]][0] - m_centroid[0]);
   covariance(0,0) /= (nvertsd);
   if (fabs(m_covariance(0,0)) < EPS)
      m_covariance(0,0) = 0.0;

   covariance(1,1) = 0.0;
   for (size_t k = 0; k < nverts; k++)
      covariance(1,1) += (m_root->m_points[m_indexes[k]][1] - m_centroid[1]) * (m_root->m_points[m_indexes[k]][1] - m_centroid[1]);
   covariance(1,1) /= (nvertsd);
   if (fabs(m_covariance(1,1)) < EPS)
      m_covariance(1,1) = 0.0;

   covariance(2,2) = 0.0;
   for (size_t k = 0; k < nverts; k++)
      covariance(2,2) += (m_root->m_points[m_indexes[k]][2] - m_centroid[2]) * (m_root->m_points[m_indexes[k]][2] - m_centroid[2]);
   covariance(2,2) /= (nvertsd);
   if (fabs(m_covariance(2,2)) < EPS)
      m_covariance(2,2) = 0.0;

   covariance(1,0) = 0.0;
   for (size_t k = 0; k < nverts; k++)
      covariance(1,0) += (m_root->m_points[m_indexes[k]][1] - m_centroid[1]) * (m_root->m_points[m_indexes[k]][0] - m_centroid[0]);
   covariance(1,0) /= (nvertsd);
   if (fabs(m_covariance(1,0)) < EPS)
      m_covariance(1,0) = 0.0;

   covariance(2,0) = 0.0;
   for (size_t k = 0; k < nverts; k++)
      covariance(2,0) += (m_root->m_points[m_indexes[k]][2] - m_centroid[2]) * (m_root->m_points[m_indexes[k]][0] - m_centroid[0]);
   covariance(2,0) /= (nvertsd);
   if (fabs(m_covariance(2,0)) < EPS)
      m_covariance(2,0) = 0.0;

   covariance(2,1) = 0.0;
   for (size_t k = 0; k < nverts; k++)
      covariance(2,1) += (m_root->m_points[m_indexes[k]][2] - m_centroid[2]) * (m_root->m_points[m_indexes[k]][1] - m_centroid[1]);
   covariance(2,1) /= (nvertsd);
   if (fabs(m_covariance(2,1)) < EPS)
      m_covariance(2,1) = 0.0;

   covariance(0,2) = covariance(2,0);
   covariance(0,1) = covariance(1,0);
   covariance(1,2) = covariance(2,1);
   
   return covariance;
}

void octree_t::least_variance_direction()
{
   m_covariance = fast_covariance_matrix();

   dlib::eigenvalue_decomposition< dlib::matrix<double,3,3> > eigenvalue_decomp(m_covariance);
   dlib::matrix<double> eigenvalues_vector = eigenvalue_decomp.get_real_eigenvalues();

   int min_index = 0, max_index = 0, middle_index = 0;

   if (eigenvalues_vector(1) < eigenvalues_vector(min_index)) {
      min_index = 1;
   } else if (eigenvalues_vector(1) > eigenvalues_vector(max_index)) {
      max_index = 1;
   }

   if (eigenvalues_vector(2) < eigenvalues_vector(min_index)) {
      min_index = 2;
   } else if (eigenvalues_vector(2) > eigenvalues_vector(max_index)) {
      max_index = 2;
   }

   while (middle_index==min_index || middle_index==max_index) middle_index++;

   variance1 = eigenvalues_vector(min_index);
   variance2 = eigenvalues_vector(middle_index);
   variance3 = eigenvalues_vector(max_index);

   dlib::matrix<double> eigenvectors_matrix = eigenvalue_decomp.get_pseudo_v();

   normal1 = Vector4d(eigenvectors_matrix(0, min_index),    eigenvectors_matrix(1, min_index),    eigenvectors_matrix(2, min_index));
   normal2 = Vector4d(eigenvectors_matrix(0, middle_index), eigenvectors_matrix(1, middle_index), eigenvectors_matrix(2, middle_index));
   normal3 = Vector4d(eigenvectors_matrix(0, max_index),    eigenvectors_matrix(1, max_index),    eigenvectors_matrix(2, max_index));
}

double octree_t::distance2plane( Vector4d &point )
{
   return abs((point - m_centroid) & normal1.Normalized());
}

void octree_t::get_nodes( std::vector< octree_t*> &nodes )
{
   if (m_children != NULL) 
   {
      for (short i = 0; i < 8 ; i++)
      {
         m_children[i].get_nodes(nodes);
      }
   } else {
      if (coplanar) {
         nodes.push_back(this);
      }
   }
}

void octree_t::print_points()
{
   glPointSize(4.0);
   glBegin(GL_POINTS);
   glNormal3d(1.0,0.0,0.0);
   glColor3dv(color);
   for (size_t i = 0; i < m_indexes.size() ; i++)
   {
      glVertex3d(m_root->m_points[m_indexes[i]].x, m_root->m_points[m_indexes[i]].y, m_root->m_points[m_indexes[i]].z);
   }
   glEnd();
   glPointSize(1.0);
}

void octree_t::show( bool type, int height )
{
   height--;
   if (height == 0) {
      glColor3dv(color);
      if (type) {
         glPushMatrix();
         glTranslated(m_middle.x, m_middle.y, m_middle.z);
         if (color == Vector4d(0.5,0.5,0.5)) {
            glutWireCube(m_size);
         } else {
            glutSolidCube(m_size);
         }
         glPopMatrix();
      } else {
         print_points();
      }
   }
   if (m_children != NULL) 
   {
      for (short i = 0; i < 8 ; i++)
      {
         m_children[i].show(type, height);
      }
   }
}

void octree_t::show( bool type)
{
   if (coplanar) {
         glColor3dv(color);
         if (type) {
            glPushMatrix();
            glTranslated(m_middle.x, m_middle.y, m_middle.z);
            
            glutWireCube(m_size);
            /*if (color != Vector4d(0.5,0.5,0.5)) 
               glutWireCube(m_size);
            else {
               glutSolidCube(m_size);
            }*/
            
         
            //glutSolidCube(m_size);
            glPopMatrix();
         } else {
            print_points();
         }
      }

   if (m_children != NULL) 
   {
      for (short i = 0; i < 8 ; i++)
      {
         m_children[i].show(type);
      }
   }
}




