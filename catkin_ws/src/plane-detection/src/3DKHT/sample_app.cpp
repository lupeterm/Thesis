#include "sample_app.h"
#include "File.h"
#include "hough.h"
#include "plane_t.h"
#include "accumulatorball_t.h"
#include "octree_t.h"
#include "settings.h"
#include "Utils.h"
#include <chrono>
#include <vector>
#include <random>

#include "reader_file.h"

std::chrono::system_clock::time_point fps_time;
std::vector<Vector4d> points;
std::vector<Vector4d> colors;
std::vector<Vector4d> color_map;
std::vector<plane_t> planes_out;

hough_settings settings;
accumulatorball_t * accum;
octree_t father;

double size = 0.6;
double max_distance = 0.0;
bool show_settings[10];
bool realtime_cond = true;
int index_plane = 0;
int plane_value = 0;
int number_planes_show = 1;
int rho_index_accumulator = 50;
int alturaoc = 1;
int cont_frames = 1;
int octree_height = 0;

void text(int x, int y, char *t)
{
    int tam = (int)strlen(t);
    int c;
    
    for(c=0; c<tam && c< 5; c++)
    {
      glRasterPos2i(x + c*10, y);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, t[c]);
    }
}

sample_app::sample_app(int width, int height) : Canvas(width, height)
{

   Vector4d color1 = Vector4d::Blue();
   Vector4d color2 = Vector4d::Cyan();
   Vector4d color3 = Vector4d::Green();
   Vector4d color4 = Vector4d::Yellow();
   Vector4d color5 = Vector4d::Red();

   double n_cores = 1000.0;
   for (int i = 0 ; i < n_cores ; i ++) {
      color_map.push_back(color2 * (double)(i)/n_cores + color1 * (double)(n_cores-i)/n_cores);
   }
   for (int i = 0 ; i < n_cores ; i ++) {
      color_map.push_back(color3 * (double)(i)/n_cores + color2 * (double)(n_cores-i)/n_cores);
   }
   for (int i = 0 ; i < n_cores ; i ++) {
      color_map.push_back(color4 * (double)(i)/n_cores + color3 * (double)(n_cores-i)/n_cores);
   }
   for (int i = 0 ; i < n_cores ; i ++) {
      color_map.push_back(color5 * (double)(i)/n_cores + color4 * (double)(n_cores-i)/n_cores);
   }
  
   for (int i = 0; i < 10 ; i++)
   {
   	show_settings[i] = false;
   }
   show_settings[0] = true;
   show_settings[4] = true;
   hold = false;

   myLighting = new Lighting();
   myLighting->setPosition(0,0,10000);

   load_point_cloud(settings, father);

   Vector4d centroid = father.m_centroid / father.m_points.size();

   for (Vector4d & v : father.m_points)
   {
      v -= centroid;
      max_distance = max(max_distance,abs(v.x));
      max_distance = max(max_distance,abs(v.y));
      max_distance = max(max_distance,abs(v.z));
      settings.max_point_distance = max(settings.max_point_distance,v.GetLength());
   }

   father.m_centroid = Vector4d();
   father.m_size = max_distance * 2.0;

   myCamera = new CamManager(Vector4d(settings.max_point_distance,settings.max_point_distance,settings.max_point_distance), father.m_centroid);
   
#ifndef realtime
   auto t_start = std::chrono::high_resolution_clock::now();
   accum = kht3d(planes_out, father, settings);
   auto t_elasped = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t_start).count()/1000.0;
   std::cout << "Hough Transform:                Total ["  << t_elasped << "]   "  << 1.0/t_elasped << " fps\n";
   std::cout << "3-D KHT ended: " << planes_out.size() << " planes found" << std::endl;
#endif

}

void sample_app::update() 
{
}

long long t_elasped = 0 ;
void sample_app::render()
{

#ifdef realtime

	father.m_centroid = Vector4d();

	planes_out.clear();
	father.clear();

	fps_time = std::chrono::high_resolution_clock::now();
	accum = kht3d(planes_out, father, settings);
	t_elasped += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - fps_time).count();

	std::cout << cont_frames++ << " " << (double)t_elasped / (double)cont_frames / 1000.0 << std::endl;

	std::cout << "  planes: " << planes_out.size() << std::endl;

	delete accum;

	if (cont_frames > 100) {
		Sleep(3000);
		exit(0);
	}

	return;

#endif


   glClearColor(1.0f, 1.0f, 1.0f, 1.f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);


#ifndef debugplanes

      // Unfolded slices of the accumulator 

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glViewport(0,0,1920,1000);
      gluOrtho2D(0, 1920, 1020, 0);
      glMatrixMode(GL_MODELVIEW);
      glPolygonOffset(1.0, 1.0);
      glDisable(GL_DEPTH_TEST);
   
      double cell_size_y = 1000.0 / (double)accum->m_data.size();

      for (int j = 0; j < accum->m_data.size() ; j++)
      {
         for (int i = 0; i < accum->m_data[j].size() ; i++)
         {
            int c, border = 5;
            double cell_size_x = 1900 / (double)accum->m_data[j].size();

            if (accum->m_data[j][i] != NULL && accum->m_data[j][i]->bins[rho_index_accumulator].bin > 0.0)
            {

               c = accum->m_data[j][i]->bins[rho_index_accumulator].bin;
               double factor = min((double)c / size, 1.0);
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
               /*if (accum->m_data[j][i]->bins[rho_index_accumulator].top)
                  glColor3d( 1.0 ,1.0, 0.0);*/
                if (accum->m_data[j][i]->bins[rho_index_accumulator].peak)
                  glColor3d( 0.0 ,1.0, 0.0);
               /*else if (accum->m_data[j][i]->bins[rho_index_accumulator].visited)
                  glColor3d( 0.0 ,0.0, 1.0);*/
               else
                  glColor4dv(color_map[std::min((int)(factor*color_map.size()),(int)color_map.size()-1)]);//glColor3d( 255.0 ,1.0 - c/10.0, 1.0 - c/10.0);
               glBegin(GL_QUADS);
               glVertex3d((border + cell_size_x*i), (border + cell_size_y*j),-1);
               glVertex3d((border + cell_size_x*(i+1)), (border + cell_size_y*j),-1);
               glVertex3d((border + cell_size_x*(i+1)), (border + cell_size_y*(j+1)),-1);
               glVertex3d((border + cell_size_x*i), (border + cell_size_y*(j+1)),-1);
               glEnd();

               glColor3f(0.0,0.0,0.0);
               text(border + cell_size_x*i + 2, border + cell_size_y*(j+1)-2, (char*)toString(accum->m_data[j][i]->bins[rho_index_accumulator].bin).c_str());
            }

            if (accum->m_data[j][i] != NULL)
               glColor3d( 0.65, 0.65, 0.65);
            else
               glColor3d( 0.8, 0.8, 0.8);
         
            glBegin(GL_LINE_STRIP);
            glVertex2d(border + cell_size_x*i, border + cell_size_y*j);
            glVertex2d(border + cell_size_x*(i+1), border + cell_size_y*j);
            glVertex2d(border + cell_size_x*(i+1), border + cell_size_y*(j+1));
            glVertex2d(border + cell_size_x*i, border + cell_size_y*(j+1));
            glVertex2d(border + cell_size_x*i, border + cell_size_y*j);
            glVertex2d(border + cell_size_x*(i+1), border + cell_size_y*j);
            glVertex2d(border + cell_size_x*(i+1), border + cell_size_y*(j+1));
            glVertex2d(border + cell_size_x*i, border + cell_size_y*(j+1));
            glEnd();

         }
      }
  
#endif
 
#ifdef debugplanes

   glMatrixMode(GL_PROJECTION);
   glViewport(0,0,_width,_height);
   glLoadIdentity();

   gluPerspective(60.0, (double)((double)_width/(double)_height), 0.1, 1000000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
   myCamera->render();
   glEnable(GL_DEPTH_TEST);
 
   /*
   glEnable(GL_CULL_FACE);
   glCullFace(GL_FRONT);
   glFrontFace(GL_CCW );
   glutSolidSphere(1000,30,30);
   glutWireSphere(1000,30,30);
   */

   // point cloud 
   if (show_settings[0]) 
   {
      glPointSize(1);
      glColor3f(0.3,0.3,0.3);
      if (show_settings[4])
         glEnableClientState(GL_COLOR_ARRAY);
	   glEnableClientState(GL_VERTEX_ARRAY);
      if (show_settings[4])
         glColorPointer(4, GL_DOUBLE, 0, &father.m_colors[0]);
      glVertexPointer(4, GL_DOUBLE, 0, &father.m_points[0]);
      glDrawArrays(GL_POINTS, 0, father.m_points.size());
      glDisableClientState(GL_VERTEX_ARRAY);	
      if (show_settings[4])
         glDisableClientState(GL_COLOR_ARRAY);	
   }

   // planes
   if (show_settings[1])
   {
	   for (unsigned int i = 0; i < (unsigned int)number_planes_show && i < planes_out.size(); i++)
	   {
		   planes_out[i].draw(size, false, false, (plane_value == i));
	   }
   }
   
   // octree
   if (show_settings[2]) 
   {     
      //father.show(true, octree_height);
      father.show(true);
   }
   if (show_settings[3]) 
   {

      double t,p,r;

	  for (unsigned int j = 0; j < accum->m_data.size(); j++)
	   {
		   for (unsigned int i = 0; i < accum->m_data[j].size(); i++)
		   {
            double thet = (double)i/(double)accum->m_data[j].size();
            
            for (int rho = 0 ; rho < accum->m_rho_length; rho++)
            {
               if (accum->m_data[j][i] == NULL) break;
               accum->get_values(t,p,r,thet,j,rho);
               accum_cell_t &cell = accum->at(thet,j,rho);

               if (cell.bin == 0) continue; 

               Vector4d vec;
               accum->spherical_to_cartesian(vec, t,p,r);
               
               double factor = min((double)cell.bin / size, 1.0);

               glPointSize(6.0);
               glBegin(GL_POINTS);
               glColor4dv(color_map[std::min((int)(factor*color_map.size()),(int)color_map.size()-1)]);
               glVertex3d(vec.x,vec.y,vec.z);
               glEnd();

            }
		   }
	   }
      glPopMatrix();
   }
#endif


}
void sample_app::mouseMotion( int x, int y )
{
   if (hold) myCamera->onMouseHold(x,y);
}
void sample_app::mouseClick( int button, int state, int x, int y ) 
{
   if (state==0) hold = true;
   else hold = false;
   myCamera->onMouseClick(button,x,y);
}
void sample_app::specialKeyboardDown( int key, int x, int y ) 
{
   
   switch (key) {
   case 101:
      planes_out[plane_value].m_scale.x+=1;
      break;
   case 102:
      planes_out[plane_value].m_scale.x-=1;
      break;
   case 103:
      planes_out[plane_value].m_scale.y+=1;
      break;
   case 104:
      planes_out[plane_value].m_scale.y-=1;
      break;
   case 105:
      planes_out[plane_value].m_scale.z+=1;
      break;
   case 106:
      planes_out[plane_value].m_scale.z-=1;
      break;
   case 107:
      planes_out[plane_value].m_scale.w+=1;
      break;
   case 108:
      planes_out[plane_value].m_scale.w-=1;
      break;
   case 109:
      planes_out[plane_value].m_rotate += PI/200.0;
      break;
   case 110:
      planes_out[plane_value].m_rotate -= PI/200.0;
      break;
   case 201:
      number_planes_show = min((int)(planes_out.size()),number_planes_show+1);
      octree_height++;
      break;
   case 203:
      number_planes_show = max(number_planes_show-1,1);
      octree_height = max(octree_height-1,0);
      break;
   case 202:
      index_plane++;
      plane_value = min((int)(number_planes_show-1),plane_value+1);
      cout << plane_value << "  ";
      cout << planes_out[plane_value].ti << "/" << planes_out[plane_value].pi << "/"<< planes_out[plane_value].ri <<  "    " << round(planes_out[plane_value].m_theta*RADIANS_TO_DEGREES) << "/" << round(planes_out[plane_value].m_phi*RADIANS_TO_DEGREES) << "  # votes " << planes_out[plane_value].votes << " # import " << planes_out[plane_value].representativeness;
      if (!planes_out[plane_value].m_showing) cout << "  -> deleted";
      std::cout << std::endl;
      break;
   case 200:
      index_plane--;
      plane_value = max(plane_value-1,0);
      cout << plane_value << "  ";
      cout << planes_out[plane_value].ti << "/" << planes_out[plane_value].pi << "/"<< planes_out[plane_value].ri <<  "    " << round(planes_out[plane_value].m_theta*RADIANS_TO_DEGREES) << "/" << round(planes_out[plane_value].m_phi*RADIANS_TO_DEGREES) << "  # votes " << planes_out[plane_value].votes << " # import " << planes_out[plane_value].representativeness;
      if (!planes_out[plane_value].m_showing) cout << "  -> deleted";
      std::cout << std::endl;
      break;
   }
}

void sample_app::keyboardDown( unsigned char key, int x, int y ) 
{
   myCamera->onKeyboard(key, true);
   switch (key) {
   case 27:
      exit(0);
      break;
   case '*':
      size+=0.1;
      break;
   case 43:
      size*=2.0;
      rho_index_accumulator = min(accum->m_rho_length-1,rho_index_accumulator+1);
      std::cout << "+ " << size << " " << rho_index_accumulator<< std::endl;
      break;
   case 45:
      size*=0.5;
      rho_index_accumulator = max(0,rho_index_accumulator-1);
      std::cout <<  "- " << size << " " << rho_index_accumulator<< std::endl;
      break;
   case 127:
      planes_out[plane_value].m_showing = !planes_out[plane_value].m_showing;
      break;
   case '\\':
      realtime_cond = !realtime_cond;
      break;
   case 'o':
      alturaoc++;
      break;
   case 'p':
      alturaoc--;
      break;
   }
   if ((int)key >= 48 && (int)key <= 57)
   {
      show_settings[(int)key - 48] = !show_settings[(int)key - 48];
   }
}
void sample_app::keyboardUp( unsigned char key, int x, int y ) 
{
   myCamera->onKeyboard(key, false);
}
void sample_app::reshape( int w, int h ) 
{
   _width = w;
   _height = h;
}


