#include "main_thrd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <synchapi.h>
#include <windows.h>
#include "if.h"
#include "thrd_data.h"
#include "wrk_thrd.h"

unsigned int
MainThrd(void *arg)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  void *wrk_thrd;
  struct ThrdData *thrd_data;
  struct Request *req;
  int i;
  unsigned int result;

  printf("[%ld] Main thread started.\n%d worker thread(s) will be created.  "
      "Waiting while worker threads are being created.\n", thrd_id,
      kNumberOfThreads);
  wrk_thrd = malloc(kNumberOfThreads * sizeof wrk_thrd);
  if (NULL != wrk_thrd)
  {
    memset(wrk_thrd, 0, kNumberOfThreads * sizeof wrk_thrd);
    thrd_data = (struct ThrdData *)arg;
    thrd_data->kill = CreateEventW(NULL, TRUE, FALSE, NULL);
    thrd_data->wrk_thrds_run = 0;
    for (i = 0; kNumberOfThreads > i; ++i)
    {
      *((void **)wrk_thrd + i) = (void *)_beginthreadex(NULL, 0, WrkThrd,
          thrd_data, 0, NULL);
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
    WaitForMultipleObjects(kNumberOfThreads, wrk_thrd, TRUE, INFINITE);
    for (i = 0; kNumberOfThreads > i; ++i)
    {
      CloseHandle(*((void **)wrk_thrd + i));
    }
    if (NULL != thrd_data->req_queue_head)
    {
      printf("%zu unprocessed request(s) remain in the queue.  Remove them.\n",
          SzOfReqQueue(thrd_data->req_queue_head));
    }
    ClearReqQueue(&thrd_data->req_queue_head, &thrd_data->req_queue_tail);
    FiniRequests();
    CloseHandle(thrd_data->kill);
    free(wrk_thrd);
    result = 0;
  }
  else
  {
    result = 1;
  }
  printf("[%ld] Main thread exited.\n", thrd_id);
  return result;
}
