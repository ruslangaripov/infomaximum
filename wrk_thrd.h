#ifndef WRK_THRD_H_
#define WRK_THRD_H_

#include <threadpoolapiset.h>

extern void __stdcall WrkThrdCallback(TP_CALLBACK_INSTANCE *inst, void *ctx,
    TP_WORK *wrk);

#endif  /* not WRK_THRD_H_ */
