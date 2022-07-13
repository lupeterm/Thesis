#pragma once

#include <GL/gl.h>
#include <GL/glut.h>

#include "SwissArmyKnife/Mathematic.h"
#include "SwissArmyKnife/Vector4d.h"
#include "SwissArmyKnife/Matrix4d.h"
#include "octree_t.h"

class octree_t;

class plane_t
{
public:
   plane_t(void) {}
   ~plane_t(void) {}

   inline void calculate()
   {

      m_normal.x = sin(m_phi) * cos(m_theta);
      m_normal.y = sin(m_phi) * sin(m_theta);
      m_normal.z = cos(m_phi);
      m_position = m_normal * m_rho;

      Vector4d c_u = Vector4d(0.0, 0.0, 1.0);
      if (c_u == m_normal)
         c_u = Vector4d(1.0, 0.0, 0.0);

      m_cross = (c_u * m_normal).Normalized();
      m_cross2 = m_normal * m_cross;

      m_centroid = Vector4d(0, 0, 0);
      m_scale = Vector4d(0, 0, 0, 0);
      m_showing = true;
      m_rotate = 0.0;
      int cont = 0;
      for (unsigned int i = 0; i < nodes.size(); i++)
      {
         cont += nodes[i]->m_indexes.size();
         for (unsigned int j = 0; j < nodes[i]->m_indexes.size(); j++)
         {
            m_centroid += nodes[i]->m_root->m_points[nodes[i]->m_indexes[j]];
         }
      }
      if (cont != 0)
         m_centroid /= (double)cont;

      m_desloc = m_centroid - m_position;
      if (m_desloc.GetLength() != 0)
      {
         Vector4d normproj = (m_desloc.Normalized() & m_normal) * m_desloc.GetLength() * m_normal;
         m_desloc -= normproj;
      }
   }

   inline bool operator<(plane_t p) { return (representativeness > p.representativeness); }

   inline void draw(double size, bool type, bool pc, bool selected)
   {

      if (selected)
      {
         glColor3d(0.0, 1.0, 0.0);
         glPointSize(6.0);
      }
      else
      {
         glColor3d(0.0, 0.0, 0.5);
         glPointSize(5.0);
      }

      if (pc)
      {
         if (type)
         {
            for (octree_t *node : nodes)
            {
               glPushMatrix();
               glTranslated(node->m_middle.x, node->m_middle.y, node->m_middle.z);
               glutSolidCube(node->m_size + 0.5);
               glPopMatrix();
            }
         }
         else
         {
            for (octree_t *node : nodes)
            {
               glBegin(GL_POINTS);
               for (size_t i = 0; i < node->m_indexes.size(); i++)
               {
                  glVertex3d(node->m_root->m_points[node->m_indexes[i]].x, node->m_root->m_points[node->m_indexes[i]].y, node->m_root->m_points[node->m_indexes[i]].z);
               }
               glEnd();
            }
         }
      }

      if (m_showing)
      {
         glPushMatrix();
         {
            glBegin(GL_QUADS);
            if (selected)
               glColor4d(0.0, 0.0, 0.0, 0.6);
            else
               glColor4d(m_color.r, m_color.g, m_color.b, 0.4);

            Vector4d cross = Matrix4d::Rotation(m_rotate, m_normal) * m_cross;
            Vector4d cross2 = Matrix4d::Rotation(m_rotate, m_normal) * m_cross2;

            glNormal3dv(m_normal);
            glVertex3dv((m_position + cross * size + cross2 * size + m_desloc + cross * m_scale.x + cross2 * m_scale.z).data);
            glVertex3dv((m_position + cross * size - cross2 * size + m_desloc + cross * m_scale.x + cross2 * m_scale.w).data);
            glVertex3dv((m_position - cross * size - cross2 * size + m_desloc + cross * m_scale.y + cross2 * m_scale.w).data);
            glVertex3dv((m_position - cross * size + cross2 * size + m_desloc + cross * m_scale.y + cross2 * m_scale.z).data);

            glEnd();
         }
         glPopMatrix();
      }
   }

   double distance2plane(Vector4d &point)
   {
      return abs((point - m_position) & m_normal.Normalized());
   }

   double m_theta;
   double m_phi;
   double m_rho;

   Vector4d m_cross;
   Vector4d m_cross2;
   Vector4d m_position;
   Vector4d m_centroid;
   Vector4d m_normal;
   Vector4d m_desloc;
   Vector4d m_scale;
   Vector4d m_color;

   std::vector<Vector4d> m_points;

   double ti, m_rotate;
   int pi, ri;
   bool m_showing;

   std::vector<octree_t *> nodes;
   ACCUM_BIN_TYPE votes;
   double representativeness;
};