/*******************************************************************************
 * Name            : threadable.hpp
 * Project         : rcppsw
 * Module          : multithread
 * Description     : Common threading functionality
 * Creation Date   : 06/18/15
 * Copyright       : Copyright 2015 John Harwell, All rights reserved
 *
 ******************************************************************************/

#ifndef INCLUDE_THREADABLE_HPP_
#define INCLUDE_THREADABLE_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <pthread.h>
#include "rcsw/include/al/altypes.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace rcppsw {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class threadable {
 public:
  /* data members */

  /* member functions */
  threadable(void) : thread_run_(false), thread_(), arg_(NULL) {}
  virtual ~threadable(void) {}

  virtual void* thread_main(void* arg) = 0;
  virtual status_t start(void* arg, int core = -1);
  virtual void term(void* ret) {
    thread_run_ = false;
    pthread_exit(ret);
  }

  bool terminated(void) { return (false == thread_run_); }
  virtual int join(void) {
    int ret;
    pthread_join(thread_, (void**)&ret);
    return ret;
  }

 protected:
  pthread_t self(void) { return thread_; }

  /* operators */
  threadable(const threadable&& other) : thread_run_(other.thread_run_),
                                         thread_(other.thread_),
                                         arg_(other.arg_) {}

 private:
  /* non-member functions */
  static void* entry_point(void* this_p) {
    threadable* pt = static_cast<threadable*>(this_p);
    return pt->thread_main(pt->arg_);
  } /* entry_point() */

  /* operators */
  const threadable& operator=(const threadable& rhs) = delete;
  threadable(const threadable& other) = delete;

  /* data members */
  bool thread_run_;
  pthread_t thread_;
  void *arg_;
};

} /* namespace rcppsw */

/*******************************************************************************
 * Operater Definitions
 ******************************************************************************/

#endif /* INCLUDE_THREADABLE_HPP_ */
