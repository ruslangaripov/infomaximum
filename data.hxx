#ifndef DATA_HXX_
#define DATA_HXX_

#include <condition_variable>
#include <deque>
#include <mutex>
#include "if.hxx"

struct Data
{
  std::mutex mtx;
  std::condition_variable reqs_is_ne;
  std::condition_variable wrk_thrds_are_ready;
  std::deque<Request *> reqs;
  int wrk_thrds_run;
  bool kill;
};

#endif  /* DATA_HXX_ */
