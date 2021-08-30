#include "main_thrd.h"
#include <stdio.h>
#include <synchapi.h>
#include <windows.h>
#include "if.h"
#include "thrd_data.h"
#include "wrk_thrd.h"

static void DispatchRequests(struct ThrdData *thrd_data);

void
DispatchRequests(struct ThrdData *thrd_data)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  struct Request *req;
  TP_WORK *wrk_thrd;
  int i;

  printf("[%ld] Main thread started.\n%d worker thread(s) will be created.  "
      "Waiting while worker threads are being created.\n", thrd_id,
      kNumberOfThreads);
  thrd_data->kill = CreateEventW(NULL, TRUE, FALSE, NULL);
  thrd_data->wrk_thrds_run = 0;
  wrk_thrd = CreateThreadpoolWork(WrkThrdCallback, thrd_data,
      thrd_data->tp_clbk_env);
  if (NULL != wrk_thrd)
  {
    for (i = 0; kNumberOfThreads > i; ++i)
    {
      SubmitThreadpoolWork(wrk_thrd);
    }
    AcquireSRWLockExclusive(&thrd_data->mtx);
    while (kNumberOfThreads != thrd_data->wrk_thrds_run)
    {
      SleepConditionVariableSRW(&thrd_data->wrk_thrds_are_ready,
          &thrd_data->mtx, INFINITE, 0);
    }
    ReleaseSRWLockExclusive(&thrd_data->mtx);
    printf("Worker threads were created.  Extract requests.\n");

    InitRequests();
    while (NULL != (req = GetRequest()))
    {
      AcquireSRWLockExclusive(&thrd_data->mtx);
      EnqueueReq(&thrd_data->req_queue_head, &thrd_data->req_queue_tail, req);
      ReleaseSRWLockExclusive(&thrd_data->mtx);
      WakeConditionVariable(&thrd_data->reqs_is_ne);
    }

    SetEvent(thrd_data->kill);
    WakeAllConditionVariable(&thrd_data->reqs_is_ne);
    AcquireSRWLockExclusive(&thrd_data->mtx);
    while (0 != thrd_data->wrk_thrds_run)
    {
      SleepConditionVariableSRW(&thrd_data->wrk_thrds_killed, &thrd_data->mtx,
          INFINITE, 0);
    }
    ReleaseSRWLockExclusive(&thrd_data->mtx);
    if (NULL != thrd_data->req_queue_head)
    {
      printf("%zu unprocessed request(s) remain in the queue.  Remove them.\n",
          SzOfReqQueue(thrd_data->req_queue_head));
    }
    ClearReqQueue(&thrd_data->req_queue_head, &thrd_data->req_queue_tail);
    FiniRequests();
  }
  printf("[%ld] Main thread exited.\n", thrd_id);
  SetEvent(thrd_data->end_program);
}

void
MainThrdCallback(TP_CALLBACK_INSTANCE *inst, void *ctx, TP_WORK *wrk)
{
  (void)wrk;

  CallbackMayRunLong(inst);
  if (NULL != ctx)
  {
    DispatchRequests((struct ThrdData *)ctx);
  }
}
