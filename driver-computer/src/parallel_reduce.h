// Penn Parallel Primitives Library
// Author: Prof. Milo Martin
// University of Pennsylvania
// Spring 2010

#ifndef PPP_REDUCE_H
#define PPP_REDUCE_H

#include "ppp.h"
#include "Task.h"
#include "TaskGroup.h"

namespace ppp {

  namespace internal {

    template <typename T>
    class ReduceTask: public ppp::Task {
    public:
      T  m_sum;

      ReduceTask(T* array, int64_t left, int64_t right, int64_t grainsize) { 
        m_array = array;
        m_left = left;
        m_right = right;
        m_grainsize = grainsize;
      }
      virtual ~ReduceTask(){};
      
      void execute() {
        PPP_DEBUG_MSG("Execute: [" + to_string(m_left) + ", " + to_string(m_right) + "]");
        assert(m_left < m_right);

        if (m_right-m_left <= 1) {
          m_sum = m_array[m_left];
          return;
        }
        
        if (m_right-m_left < m_grainsize) {
          //PPP_DEBUG_MSG("std::sort: [" + to_string(m_left) + ", " + to_string(m_right) + "]");
          //PPP_DEBUG_MSG("std::sort: [" + to_string(&m_array[m_left]) + ", " + to_string(&m_array[m_right]) + "]");
          //std::sort(&m_array[m_left], &m_array[m_right]); 
          m_sum = T(0);
          for (int i=m_left; i<m_right; i++) {
            m_sum = m_sum + m_array[i];
          }
          return;
        }
        
        int64_t pivot = (m_left + m_right)/2;
        assert(pivot < m_right);
        
        //PPP_DEBUG_MSG("Split: [" + to_string(m_left) + ", " + to_string(pivot) + "] [" +
        //              to_string(pivot) + ", " + to_string(m_right) + "]");
        ppp::TaskGroup tg;
        ReduceTask t1(m_array, m_left, pivot, m_grainsize);
        ReduceTask t2(m_array, pivot, m_right, m_grainsize);
        tg.spawn(t1);
        tg.spawn(t2);
        tg.wait();
        m_sum = t1.m_sum + t2.m_sum;
      }
    private:
      T* m_array;
      int64_t m_left;
      int64_t m_right;
      int64_t m_grainsize;
    };
  }
  
  template <typename T>
  extern inline
  T parallel_reduce(T* array, int64_t start, int64_t end, int64_t grainsize=0)
  {
    // ASSIGNMENT: make this parallel via recursive divide and conquer
    if (grainsize == 0) {
      grainsize = (end-start+1) / (get_thread_count()*4);
    }
            
    internal::ReduceTask<T> t(array, start, end, grainsize);
    t.execute();
    return t.m_sum;
    // Sequential code
    //T sum;
    //sum = T(0);
    //for (int i=start; i<end; i++) {
    //  sum = sum + array[i];
    //}
    //return sum;
  }
}

#endif
