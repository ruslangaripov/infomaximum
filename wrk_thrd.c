#include "wrk_thrd.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <synchapi.h>
#include <windows.h>
#include "if.h"
#include "thrd_data.h"

static size_t id_counter;

unsigned int
WrkThrd(void *arg)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  struct Request *req;
  struct ThrdData *thrd_data;
  size_t id;

  id = id_counter++;
  thrd_data = (struct ThrdData *)arg;
  AcquireSRWLockExclusive(&thrd_data->mtx);
  ++thrd_data->wrk_thrds_run;
  ReleaseSRWLockExclusive(&thrd_data->mtx);
  printf("[%ld] Worker thread #%zu started.\n", thrd_id, id);
  WakeConditionVariable(&thrd_data->wrk_thrds_are_ready);

  while (1)
  {
    AcquireSRWLockExclusive(&thrd_data->mtx);
    while (WAIT_TIMEOUT == WaitForSingleObject(thrd_data->kill, 0) &&
        NULL == thrd_data->req_queue_head)
    {
      SleepConditionVariableSRW(&thrd_data->reqs_is_ne, &thrd_data->mtx,
          INFINITE, 0);
    }
    if (WAIT_OBJECT_0 == WaitForSingleObject(thrd_data->kill, 0))
    {
      ReleaseSRWLockExclusive(&thrd_data->mtx);
      break;
    }
    req = DequeueReq(&thrd_data->req_queue_head);
    ReleaseSRWLockExclusive(&thrd_data->mtx);
    ProcessRequest(req);
    if (NULL != req)
    {
      free(req);
    }
  }

  printf("[%ld] Worker thread #%zu exited.\n", thrd_id, id);
  return 0;
}
