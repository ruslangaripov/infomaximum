#include "main_thrd.hxx"
#include <windows.h>
#include "if.hxx"
#include "sync/srw_lock.hxx"
#include "sync/srw_lock_guard.hxx"
#include "wrk_thrd.hxx"

void
MainThrd::Callback(TP_CALLBACK_INSTANCE *inst, void *ctx, TP_WORK *wrk)
{
  (void)wrk;

  MainThrd *this_ptr;

  CallbackMayRunLong(inst);
  if (nullptr != ctx)
  {
    this_ptr = (MainThrd *)ctx;
    this_ptr->DispatchRequests();
  }
}

void
MainThrd::DispatchRequests(void)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  WrkThrd wrk_thrd;
  std::deque<Request *>::iterator req_it;
  Request *req;
  int i;

  std::printf("[%ld] Main thread started.\n%d worker thread(s) will be "
      "created.  Waiting while worker threads are being created.\n", thrd_id,
      kNumberOfThreads);
  m_data->kill = CreateEventW(nullptr, TRUE, FALSE, NULL);
  m_data->wrk_thrds_run = 0;
  wrk_thrd.SetThrdData(m_data);
  wrk_thrd.Create(m_data->tp_env);
  for (i = 0; kNumberOfThreads > i; ++i)
  {
    wrk_thrd.Submit();
  }
  {
    SrwLockGuardExclusive<SrwLock> guard(m_data->mtx);
    while (kNumberOfThreads != m_data->wrk_thrds_run)
    {
      m_data->wrk_thrds_are_ready.Wait(m_data->mtx.Get(), INFINITE, 0);
    }
  }
  std::printf("Worker threads were created.  Extract requests.\n");
  while (nullptr != (req = GetRequest()))
  {
    {
      SrwLockGuardExclusive<SrwLock> guard(m_data->mtx);
      m_data->reqs.push_back(req);
    }
    m_data->reqs_is_ne.NotifyOne();
  }
  SetEvent(m_data->kill);
  m_data->reqs_is_ne.NotifyAll();
  {
    SrwLockGuardExclusive<SrwLock> guard(m_data->mtx);
    while (0 != m_data->wrk_thrds_run)
    {
      m_data->wrk_thrds_killed.Wait(m_data->mtx.Get(), INFINITE, 0);
    }
  }
  if (0 != m_data->reqs.size())
  {
    std::printf("%zu unprocessed request(s) remain in the queue.  Remove them."
        "\n", m_data->reqs.size());
  }
  for (req_it = m_data->reqs.begin(); m_data->reqs.cend() != req_it; ++req_it)
  {
    if (nullptr != *req_it)
    {
      delete *req_it;
    }
  }
  std::printf("[%ld] Main thread exited.\n", thrd_id);
  SetEvent(m_data->end_program);
}
