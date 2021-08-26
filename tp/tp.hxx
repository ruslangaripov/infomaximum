#ifndef TP_TP_HXX_
#define TP_TP_HXX_

#include <windows.h>
#include "unique_handle.hxx"

class TpTraits: public UniqueHandleTraits<TP_POOL *>
{
public:
  static void
  Close(Type handle) noexcept(true)
  {
    CloseThreadpool(handle);
  }
};

typedef UniqueHandle<TpTraits> Tp;

#endif /* not TP_TP_HXX_ */
