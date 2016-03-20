// Penn Parallel Primitives Library
// Author: Prof. Milo Martin
// University of Pennsylvania
// Spring 2010

#include "ppp.h"
#include "TaskGroup.h"

namespace ppp {

  namespace internal {
    // Task-based scheduling variables
    TaskQueue* g_queues_ptr = NULL;
    atomic<int> g_stop_counter;  
  }

  using namespace internal;

  void TaskGroup::spawn(Task& t) {
    assert(g_queues_ptr != NULL);
    //TaskQueue& queue = g_queues_ptr[0];  // ASSIGNMENT: use per-thread task queue with "get_thread_id()"
    TaskQueue& queue = g_queues_ptr[get_thread_id()];  // ASSIGNMENT: use per-thread task queue with "get_thread_id()"
    m_wait_counter.fetch_and_inc();
    t.setCounter(&m_wait_counter);
    queue.enqueue(&t);
  }
  
  void process_tasks(const atomic<int>* counter)
  {
    //TaskQueue& queue = g_queues_ptr[0];  // ASSIGNMENT: use per-thread task queue with "get_thread_id()"
    TaskQueue& queue = g_queues_ptr[get_thread_id()];  // ASSIGNMENT: use per-thread task queue with "get_thread_id()"

    while (counter->get() != 0) {
      PPP_DEBUG_EXPR(queue.size());
       
      // Dequeue from local queue
      //Task* task = queue.dequeue();

      // ASSIGNMENT: add task stealing
      Task* task = queue.dequeue();

      if( task == NULL )
      {
        for (int i = 0; i < get_thread_count(); i++)
        {
          if(i==get_thread_id())
            continue;
          task = g_queues_ptr[i].steal(); 
          if(task!=NULL)
            break;
        }
      }

      if (task != NULL) {
        task->execute(); // overloaded method
        task->post_execute(); // cleanup, method of base class
      }
    }
  }
}


