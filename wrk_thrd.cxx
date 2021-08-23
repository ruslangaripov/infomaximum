#include "wrk_thrd.hxx"
#include <cstdio>
#include <mutex>
#include <windows.h>
#include "if.hxx"

void
WrkThrd(std::size_t id, Data *data)
{
  Request *req;
  const unsigned long int thrd_id = GetCurrentThreadId();

  std::printf("[%ld] Worker thread #%zu started.\n", thrd_id, id);
  while (true)
  {
    {
      std::unique_lock<std::mutex> lck(data->mtx);
      while (!data->kill && data->reqs.empty())
      {
        data->reqs_is_ne.wait(lck);
      }
      if (data->kill)
      {
        break;
      }
      req = data->reqs.front();
      data->reqs.pop_front();
    }
    ProcessRequest(req);
    if (nullptr != req)
    {
      delete req;
    }
  }
  std::printf("[%ld] Worker thread #%zu exited.\n", thrd_id, id);
}
