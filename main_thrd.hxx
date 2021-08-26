#ifndef MAIN_THRD_HXX_
#define MAIN_THRD_HXX_

#include "data.hxx"
#include "tp/tp_wrk.hxx"

class MainThrd: public TpWrk
{
public:
  void
  Create(TP_CALLBACK_ENVIRON *tp_env)
  {
    BaseClass::Create(Callback, tp_env);
  }

  void
  SetThrdData(struct Data *data)
  {
    m_data = data;
  }

private:
  typedef TpWrk BaseClass;

  static void __stdcall Callback(TP_CALLBACK_INSTANCE *inst, void *ctx,
      TP_WORK *wrk);

  void DispatchRequests(void);

  struct Data *m_data;
};

#endif  /* not MAIN_THRD_HXX_ */
