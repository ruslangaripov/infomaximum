#ifndef THRD_DATA_H_
#define THRD_DATA_H_

#include <synchapi.h>
#include "req_queue.h"

struct ThrdData
{
  SRWLOCK mtx;
  CONDITION_VARIABLE reqs_is_ne;
  CONDITION_VARIABLE wrk_thrds_are_ready;
  CONDITION_VARIABLE wrk_thrds_killed;
  struct ReqQueueData *req_queue_head;
  struct ReqQueueData *req_queue_tail;
  TP_CALLBACK_ENVIRON *tp_clbk_env;
  void *end_program;
  void *kill;
  int wrk_thrds_run;
};

#endif  /* THRD_DATA_H_ */
