#ifndef TP_TP_WRK_HXX_
#define TP_TP_WRK_HXX_

#include <windows.h>

class TpWrk
{
public:
  TpWrk(void)
      : m_wrk{nullptr}, m_shutdown{nullptr}
  {
  }
  /* Schedules execution of this work on the thread pool. */
  void
  Submit()
  {
    SubmitThreadpoolWork(m_wrk);
  }

  TP_WORK *
  Get(void) const noexcept(true)
  {
    return m_wrk;
  }

protected:
  void
  Create(PTP_WORK_CALLBACK clbk, TP_CALLBACK_ENVIRON *env)
  {
    m_wrk = CreateThreadpoolWork(clbk, this, env);
  }

private:
  TP_WORK *m_wrk;
  void *m_shutdown;
};

#endif /* not TP_TP_WRK_HXX_ */
