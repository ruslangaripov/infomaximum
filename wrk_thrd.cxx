#include "wrk_thrd.hxx"
#include <windows.h>
#include "if.hxx"
#include "sync/srw_lock.hxx"
#include "sync/srw_lock_guard.hxx"

static std::size_t id_counter;

void
WrkThrd::Callback(TP_CALLBACK_INSTANCE *inst, void *ctx, TP_WORK *wrk)
{
  (void)wrk;

  WrkThrd *this_ptr;

  CallbackMayRunLong(inst);
  if (nullptr != ctx)
  {
    this_ptr = (WrkThrd *)ctx;
    this_ptr->ProcessRequests(id_counter++);
  }
}

void
WrkThrd::ProcessRequests(std::size_t id)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  Request *req;

  {
    SrwLockGuardExclusive<SrwLock> guard(m_data->mtx);
    ++m_data->wrk_thrds_run;
  }
  std::printf("[%ld] Worker thread #%zu started.\n", thrd_id, id);
  m_data->wrk_thrds_are_ready.NotifyOne();
  while (true)
  {
    {
      SrwLockGuardExclusive<SrwLock> guard(m_data->mtx);
      while (WAIT_TIMEOUT == WaitForSingleObject(m_data->kill, 0) &&
          m_data->reqs.empty())
      {
        m_data->reqs_is_ne.Wait(m_data->mtx.Get(), INFINITE, 0);
      }
      if (WAIT_OBJECT_0 == WaitForSingleObject(m_data->kill, 0))
      {
        break;
      }
      req = m_data->reqs.front();
      m_data->reqs.pop_front();
    }
    ProcessRequest(req);
    if (nullptr != req)
    {
      delete req;
    }
  }
  {
    SrwLockGuardExclusive<SrwLock> guard(m_data->mtx);
    --m_data->wrk_thrds_run;
  }
  std::printf("[%ld] Worker thread #%zu exited.\n", thrd_id, id);
  m_data->wrk_thrds_killed.NotifyOne();
}
