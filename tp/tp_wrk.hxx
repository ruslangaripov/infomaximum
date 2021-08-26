#ifndef TP_TP_WRK_HXX_
#define TP_TP_WRK_HXX_

#include <windows.h>

/* The TpWrk class defines a wrapper for the 'TP_WORK *' pointer, but because
 * implementation of this class does not use stuff of the UniqueHandle type,
 * the TpWrk class assumes that wrapped thread pool work will be released by a
 * cleanup group, and not by an instance of this class.  This class creates
 * thread pool work, owns by the 'TP_WORK *' pointer but never releases the
 * work object. */
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
