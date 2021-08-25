#ifndef SYNC_COND_VAR_HXX_
#define SYNC_COND_VAR_HXX_

#include <synchapi.h>

class CondVar
{
public:
  CondVar(void)
  {
    InitializeConditionVariable(&m_cond_var);
  }
  CondVar(const ConditionVariable &) = delete;
  CondVar &operator =(const ConditionVariable &) = delete;

  /* Gets raw pointer to the underlying condition variable object.  And just
   * like with SRW locks -- do not modify the result.  Only use the condition
   * variable functions to manage condition variables. */
  const CONDITION_VARIABLE *Get(void) const noexcept(true)
  {
    return &m_cond_var;
  }
  CONDITION_VARIABLE *Get(void) noexcept(true)
  {
    return &m_cond_var;
  }
  /* Waits on this condition variable and releases the specified `srw_lock` as
   * an atomic operation. The lock must be held in the manner specified by the
   * `flags`. When the latter is `CONDITION_VARIABLE_LOCKMODE_SHARED`, the
   * `srw_lock` must be acquired in the shared mode. Otherwise -- in the
   * exclusive mode. The `timeout` specifies time-out interval, in milliseconds.
   * This member returns if the interval elapses. When the `timeout` is zero,
   * this member tests the state of this condition variable and returns
   * immediately. If `timeout` is `INFINITE` this function's time-out interval
   * never elapses. On success this member returns a non-zero value. */
  int Wait(SRWLOCK *srw_lock, unsigned long int timeout,
      unsigned long int flags)
  {
    return SleepConditionVariableSRW(&m_cond_var, srw_lock, timeout, flags);
  }
  /* Wakes a single thread waiting on this condition variable. */
  void NotifyOne(void)
  {
    WakeConditionVariable(&m_condition_variable);
  }

private:
  CONDITION_VARIABLE m_cond_var;
};

#endif /* not SYNC_COND_VAR_HXX_ */
