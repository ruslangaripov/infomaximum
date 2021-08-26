#ifndef WRK_THRD_HXX_
#define WRK_THRD_HXX_

#include <cstddef>
#include <windows.h>
#include "data.hxx"
#include "tp/tp_wrk.hxx"

class WrkThrd: public TpWrk
{
public:
  void Create(TP_CALLBACK_ENVIRON *tp_env)
  {
    BaseClass::Create(Callback, tp_env);
  }

  void SetThrdData(struct Data *data)
  {
    m_data = data;
  }

private:
  typedef TpWrk BaseClass;

  static void __stdcall Callback(TP_CALLBACK_INSTANCE *inst, void *ctx,
      TP_WORK *wrk);

  void ProcessRequests(std::size_t id);

  struct Data *m_data;
};

#endif  /* not WRK_THRD_HXX_ */
