#ifndef TP_TP_CLN_GRP_HXX_
#define TP_TP_CLN_GRP_HXX_

#include <windows.h>
#include "unique_handle.hxx"

class TpClnGrpTraits: public UniqueHandleTraits<TP_CLEANUP_GROUP *>
{
public:
  static void
  Close(Type handle) noexcept(true)
  {
    CloseThreadpoolCleanupGroup(handle);
  }
};

typedef UniqueHandle<TpClnGrpTraits> TpClnGrp;

#endif /* TP_TP_CLN_GRP_HXX_ */
