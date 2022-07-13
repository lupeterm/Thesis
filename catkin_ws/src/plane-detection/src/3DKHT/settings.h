
#pragma once

#define debugplanes
//#define realtime

#define ACCUM_BIN_TYPE float
#define NONZERO 0.00001

class hough_settings {
public:

   hough_settings()
   {
      // Accumulator discretization
      phi_num = 30;
      rho_num = 300;
          
	  // Percentage of the number of points from the point cloud to stop subdividing
	  s_ps = 0.002;

      // relative tolerances associated with plane thickness (s_alpha) and plane isotropy (s_beta)
      max_thickness = 1.0/25.0;
      min_isotropy = 1.0/6.0;     

      // Point cloud examples (let one block uncommented) =======================================
      // Max distance is only used for coloring the point cloud after the plane detection

	  // 9 planes
      /**
      s_level = 1;
      file = "../pointclouds/Computer.txt";
      max_distance2plane = 0.025;
      /**/

	  // 9 planes
      /**
      s_level = 4;
      file = "../pointclouds/Room.txt";
      max_distance2plane = 0.2;
      /**/

	  // 11 planes
      /**
      s_level = 5;
      file = "../pointclouds/Utrecht.txt";
      max_distance2plane = 0.2;
      /**/ 

	  // 14 planes
      /**/
      s_level = 6;
      file = "../pointclouds/Museum.txt";
      max_distance2plane = 0.3;
      /**/

	  // 6 planes
      /**
      s_level = 2;
      file = "../pointclouds/Box.txt";
      max_distance2plane = 10.0;
      /**/ 

	  // 13 planes
      /**
      s_level = 7;
      file = "../pointclouds/Bremen.txt";
      max_distance2plane = 0.5;
      /**/
      
      // ========================================================================================
      
   }

   int phi_num;
   int rho_num;

   int s_level;
   int s_ms;
   double s_ps;

   double max_point_distance;
   double max_distance2plane;

   double max_thickness;
   double min_isotropy;

   std::string file;
   std::string extension;
};