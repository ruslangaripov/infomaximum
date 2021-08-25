#ifndef SYNC_SRW_LOCK_HXX_
#define SYNC_SRW_LOCK_HXX_

#include <synchapi.h>

class SrwLock
{
public:
  SrwLock(void)
  {
    InitializeSRWLock(&m_lock);
  }
  SrwLock(const SrwLock &) = delete;
  SrwLock& operator =(const SrwLock &) = delete;

  /* Gets raw pointer to the underlying slim reader/writer lock object. Remember
   * that:
   * > An SRW lock cannot be moved or copied. The process must not modify the
   * object, and must instead treat it as logically opaque. Only use the SRW
   * functions to manage SRW locks. */
  const SRWLOCK *Get(void) const noexcept(true)
  {
    return &m_lock;
  }
  SRWLOCK *Get(void) noexcept(true)
  {
    return &m_lock;
  }
  void LockExclusive(void)
  {
    AcquireSRWLockExclusive(&m_lock);
  }
  void LockShared(void)
  {
    AcquireSRWLockShared(&m_lock);
  }
  void UnlockExclusive(void) noexcept(true)
  {
    ReleaseSRWLockExclusive(&m_lock);
  }
  void UnlockShared(void) noexcept(true)
  {
    ReleaseSRWLockShared(&m_lock);
  }
  bool TryLockExclusive(void)
  {
    return 0 != TryAcquireSRWLockExclusive(&m_lock);
  }
  bool TryLockShared(void)
  {
    return 0 != TryAcquireSRWLockShared(&m_lock);
  }

private:
  SRWLOCK m_lock;
};

#endif /* not SYNC_SRW_LOCK_HXX_ */
