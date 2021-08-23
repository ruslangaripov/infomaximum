#include "main_thrd.hxx"
#include <algorithm>
#include <cstdio>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <windows.h>
#include "data.hxx"
#include "if.hxx"
#include "wrk_thrd.hxx"

void
MainThrd(int wrk_thrd_nmbr)
{
  const unsigned long int thrd_id = GetCurrentThreadId();
  std::vector<std::thread> wrk_thrds(wrk_thrd_nmbr);
  std::vector<std::thread>::iterator thrd_it;
  std::vector<std::thread>::size_type i;
  std::deque<Request *>::iterator req_it;
  Data data;
  Request *req;

  std::printf("[%ld] Main thread started.\n%d worker thread(s) will be "
      "created.\n", thrd_id, wrk_thrd_nmbr);
  data.kill = false;
  for (i = 0; wrk_thrds.size() > i; ++i)
  {
    wrk_thrds.at(i) = std::move(std::thread(WrkThrd, i, &data));
  }
  while (nullptr != (req = GetRequest()))
  {
    {
      std::unique_lock<std::mutex> lck(data.mtx);
      data.reqs.push_back(req);
    }
    data.cv.notify_one();
  }
  /* Intel SDM 3A, 8.1.1 Guaranteed atomic operations: reading or writing byte
   * is always carried out atomically. */
  data.kill = true;
  data.cv.notify_all();
  for (thrd_it = wrk_thrds.begin(); wrk_thrds.end() != thrd_it; ++thrd_it)
  {
    thrd_it->join();
  }
  if (0 != data.reqs.size())
  {
    std::printf("%zu unprocessed request(s) remains in the queue.  Remove them."
        "\n", data.reqs.size());
  }
  for (req_it = data.reqs.begin(); data.reqs.end() != req_it; ++req_it)
  {
    if (nullptr != *req_it)
    {
      delete *req_it;
    }
  }
  std::printf("[%ld] Main thread exited.\n", thrd_id);
}
