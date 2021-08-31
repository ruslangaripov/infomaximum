#ifndef THRD_DATA_H_
#define THRD_DATA_H_

#include <synchapi.h>
#include "req_queue.h"

struct ThrdData
{
  SRWLOCK mtx;
  CONDITION_VARIABLE reqs_is_ne;
  CONDITION_VARIABLE wrk_thrds_are_ready;
  struct ReqQueueData *req_queue_head;
  struct ReqQueueData *req_queue_tail;
  void *kill;
  int wrk_thrds_run;
};

#endif  /* THRD_DATA_H_ */
