#include "if.hxx"
#include <cstddef>
#include <cstdio>
#include <chrono>
#include <random>
#include <windows.h>

class Requests
{
public:
  Requests()
    : m_reqs(kReqNmbr), m_idx(0)
  {
    std::size_t i;

    for (i = 0; m_reqs.size() > i; ++i)
    {
      m_reqs.at(i) = new Request((int)i);
    }
  }

  Request *
  Get(void)
  {
    return m_reqs.size() > m_idx ? m_reqs.at(m_idx++) : nullptr;
  }

private:
  static const std::size_t kReqNmbr = 10;

  std::vector<Request *> m_reqs;
  std::size_t m_idx;
};

/* The GetRequest() function is called by single main thread only.  The
 * ProcessRequest() one -- by several worker threads.  Both functions imitate
 * data processing by suspending a thread for random time span.  To force a
 * situation when there are no more requests the main thread can get, but the
 * worker threads still have work to do (Data::reqs is not empty), "synthetic"
 * delays in the main thread are much shorter then in the worker threads. */

Request *
GetRequest(void) noexcept(true)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  static Requests reqs;

  std::random_device rd;
  std::uniform_int_distribution<int> d(10, 100);
  Request *req;
  int sleep;

  req = reqs.Get();
  if (nullptr != req)
  {
    sleep = d(rd);
    std::printf("[%ld] Main thread imitates creation of request #%d for %d "
        "ms.\n", thrd_id, req->GetId(), sleep);
    Sleep(sleep);
  }
  return req;
}

void
ProcessRequest(const Request *request) noexcept(true)
{
  (void)request;

  const unsigned long int thrd_id = GetCurrentThreadId();

  std::random_device rd;
  std::uniform_int_distribution<int> d(100, 1000);
  int sleep;

  sleep = d(rd);
  /* TODO: Check if the request is NULL pointer. */
  std::printf("[%ld] Worker thread imitates processing of request #%d for %d "
      "ms.\n", thrd_id, request->GetId(), sleep);
  Sleep(sleep);
}
