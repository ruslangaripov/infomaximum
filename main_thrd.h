#ifndef MAIN_THRD_H_
#define MAIN_THRD_H_

#include <threadpoolapiset.h>

extern void __stdcall MainThrdCallback(TP_CALLBACK_INSTANCE *inst, void *ctx,
    TP_WORK *wrk);

#endif  /* not MAIN_THRD_H_ */
