#ifndef TP_TP_ENV_HXX_
#define TP_TP_ENV_HXX_

#include <windows.h>
#include "unique_handle.hxx"

class TpEnvTraits: public UniqueHandleTraits<TP_CALLBACK_ENVIRON *>
{
public:
  static void
  Close(Type handle) noexcept(true)
  {
    DestroyThreadpoolEnvironment(handle);
  }
};

class TpEnv: public UniqueHandle<TpEnvTraits>
{
public:
  /* Remove move constructor and move assignment operator -- we do not want to
   * move a TP_CALLBACK_ENVIRON structure around.  Copy constructor and copy
   * assignment operator are deleted by the base class. */
  TpEnv(TpEnv &&) = delete;

  TpEnv(void) noexcept(true)
      : BaseClass(&m_env)
  {
    InitializeThreadpoolEnvironment(&m_env);
  }

  TpEnv operator =(TpEnv &&) = delete;

private:
  typedef UniqueHandle<TpEnvTraits> BaseClass;

  TP_CALLBACK_ENVIRON m_env;
};

#endif /* not TP_TP_ENV_HXX_ */
