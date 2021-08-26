#ifndef UNIQUE_HANDLE_HXX_
#define UNIQUE_HANDLE_HXX_

#include <utility>

template<typename Traits>
class UniqueHandle
{
public:
  typedef Traits TraitsType;
  typedef decltype(TraitsType::Invalid()) HandleType;

  UniqueHandle(const UniqueHandle &) = delete;

  UniqueHandle(void) noexcept(true)
      : m_handle{TraitsType::Invalid()}
  {
  }

  UniqueHandle(UniqueHandle &&right) noexcept(true)
      : m_handle{right.Release()}
  {
  }

  explicit UniqueHandle(const HandleType handle) noexcept(true)
      : m_handle{handle}
  {
  }

  UniqueHandle &
  operator =(const UniqueHandle &) = delete;

  UniqueHandle &
  operator =(UniqueHandle &&right) noexcept
  {
    if (&right != this)
    {
      Reset(right.Release());
    }
    return *this;
  }

  virtual ~UniqueHandle(void) noexcept(true)
  {
    Close();
  }

  explicit operator bool(void) const noexcept(true)
  {
    return (TraitsType::Invalid() != m_handle);
  }

  bool
  operator ==(const UniqueHandle &right) const noexcept(true)
  {
    return (right.m_handle == m_handle);
  }

  bool
  operator !=(const UniqueHandle &right) const noexcept(true)
  {
    return (right.m_handle != m_handle);
  }

  HandleType
  Get(void) const noexcept(true)
  {
    return m_handle;
  }

  void
  Reset(void) noexcept(true)
  {
    Close();
  }

  bool
  Reset(const HandleType handle) noexcept(true)
  {
    if (handle != m_handle)
    {
      Close();
      m_handle = handle;
    }
    return (bool)*this;
  }

  HandleType
  Release(void) noexcept(true)
  {
    HandleType handle = m_handle;
    m_handle = TraitsType::Invalid();
    return handle;
  }

private:
  void
  Close(void) noexcept(true)
  {
    if (*this)
    {
      TraitsType::Close(m_handle);
      m_handle = TraitsType::Invalid();
    }
  }

  HandleType m_handle;
};

template<typename T>
class UniqueHandleTraits
{
public:
  typedef T Type;

  static Type
  Invalid(void) noexcept(true)
  {
    return nullptr;
  }
};

#endif /* not UNIQUE_HANDLE_HXX_ */
