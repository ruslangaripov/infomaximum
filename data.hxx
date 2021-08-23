#ifndef DATA_HXX_
#define DATA_HXX_

#include <condition_variable>
#include <deque>
#include <mutex>
#include "if.hxx"

struct Data
{
  std::mutex mtx;
  std::condition_variable cv;
  std::deque<Request *> reqs;
  bool kill;
};

#endif  /* DATA_HXX_ */
