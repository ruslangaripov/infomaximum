#ifndef SYNC_SRW_LOCK_GUARD_HXX_
#define SYNC_SRW_LOCK_GUARD_HXX_

#include <mutex>

template<typename Mutex>
class SrwLockGuardExclusive
{
public:
  typedef Mutex MutexType;

  SrwLockGuardExclusive(void) = delete;
  SrwLockGuardExclusive(const SrwLockGuardExclusive &) = delete;

  explicit SrwLockGuardExclusive(MutexType &mutex)
      : m_mutex{mutex}, m_owns{false}
  {
    m_mutex.LockExclusive();
    m_owns = true;
  }

  SrwLockGuardExclusive(MutexType &mutex, std::adopt_lock_t)
      : m_mutex{mutex}, m_owns{true}
  {
  }

  SrwLockGuardExclusive(MutexType &mutex, std::try_to_lock_t)
      : m_mutex{mutex}, m_owns{m_mutex.TryLockExclusive()}
  {
  }

  ~SrwLockGuardExclusive(void) noexcept(true)
  {
    if (m_owns)
    {
      m_mutex.UnlockExclusive();
    }
  }

  SrwLockGuardExclusive &
  operator =(const SrwLockGuardExclusive &) = delete;

  explicit operator bool(void) const noexcept(true)
  {
    return m_owns;
  }

private:
  MutexType &m_mutex;
  bool m_owns;
};

template<typename Mutex>
class SrwLockGuardShared
{
public:
  typedef Mutex MutexType;

  SrwLockGuardShared(void) = delete;
  SrwLockGuardShared(const SrwLockGuardShared &) = delete;

  explicit SrwLockGuardShared(MutexType &mutex)
      : m_mutex{mutex}, m_owns{false}
  {
    m_mutex.lockShared();
    m_owns = true;
  }

  SrwLockGuardShared(MutexType &mutex, std::adopt_lock_t)
      : m_mutex{mutex}, m_owns{true}
  {
  }

  SrwLockGuardShared(MutexType &mutex, std::try_to_lock_t)
      : m_mutex{mutex}, m_owns{m_mutex.TryLockShared()}
  {
  }

  ~SrwLockGuardShared(void) noexcept(true)
  {
    if (m_owns)
    {
      m_mutex.UnlockShared();
    }
  }

  SrwLockGuardShared &
  operator =(const SrwLockGuardShared&) = delete;

  explicit operator bool(void) const noexcept(true)
  {
    return m_owns;
  }

private:
  MutexType &m_mutex;
  bool m_owns;
};

#endif /* not SYNC_SRW_LOCK_GUARD_HXX_ */
