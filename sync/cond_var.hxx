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
  CondVar(const CondVar &) = delete;
  CondVar(CondVar &&) = delete;
  CondVar &operator =(const CondVar &) = delete;
  CondVar &operator =(CondVar &&) = delete;

  const CONDITION_VARIABLE *
  Get(void) const noexcept(true)
  {
    return &m_cond_var;
  }
  CONDITION_VARIABLE *
  Get(void) noexcept(true)
  {
    return &m_cond_var;
  }
  int
  Wait(SRWLOCK *srw_lock, unsigned long int timeout, unsigned long int flags)
  {
    return SleepConditionVariableSRW(&m_cond_var, srw_lock, timeout, flags);
  }
  void
  NotifyOne(void)
  {
    WakeConditionVariable(&m_cond_var);
  }
  void
  NotifyAll(void)
  {
    WakeAllConditionVariable(&m_cond_var);
  }

private:
  CONDITION_VARIABLE m_cond_var;
};

#endif /* not SYNC_COND_VAR_HXX_ */
