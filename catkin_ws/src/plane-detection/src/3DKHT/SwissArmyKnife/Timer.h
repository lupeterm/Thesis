#pragma once

#include <omp.h>

class Timer {
public:

   Timer() : m_start(0.0), m_end(0.0), m_elapsed(0.0), m_running(false)
   {      
   }
   
   ~Timer()
   {
   }

   void start()
   {
      if (m_running) return;
      m_start = omp_get_wtime();
      m_running = true;
   }

   double stop()
   {
      if (!m_running) return 0.0;
      m_end = omp_get_wtime();
      double c_elapsed = (m_end - m_start);
      m_elapsed += c_elapsed;
      m_start = m_end = 0.0;
      m_running = false;
      return c_elapsed;
   }

   double get_elapsed_time()
   { 
      return m_elapsed;
   }

   bool is_running()
   {
      return m_running;
   }

   void print_elapsed() 
   {
      std::cout << m_elapsed << "s" << std::endl;
   }

private:

   bool m_running;
   double m_start, m_end;
   double m_elapsed;

};