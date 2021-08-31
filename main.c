#include <stdio.h>
#include <process.h>
#include <windows.h>
#include "main_thrd.h"
#include "thrd_data.h"

static void InitThrdData(struct ThrdData *thrd_data);

void
InitThrdData(struct ThrdData *thrd_data)
{
  InitializeSRWLock(&thrd_data->mtx);
  InitializeConditionVariable(&thrd_data->reqs_is_ne);
  InitializeConditionVariable(&thrd_data->wrk_thrds_are_ready);
  InitReqQueue(&thrd_data->req_queue_head, &thrd_data->req_queue_tail);
}

int
main(void)
{
  struct ThrdData thrd_data;
  void *main_thrd;
  unsigned long int thrd_result;
  unsigned int thrd_id;

  InitThrdData(&thrd_data);
  main_thrd = (void *)_beginthreadex(NULL, 0, MainThrd, &thrd_data, 0,
      &thrd_id);
  if (NULL != main_thrd)
  {
    WaitForSingleObject(main_thrd, INFINITE);
    GetExitCodeThread(main_thrd, &thrd_result);
    if (0 != thrd_result)
    {
      printf("[%u] Failed in the main thread.\n", thrd_id);
    }
    CloseHandle(main_thrd);
  }
  return 0;
}
