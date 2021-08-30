#include <threadpoolapiset.h>
#include <windows.h>
#include "if.h"
#include "main_thrd.h"
#include "thrd_data.h"

static void InitThrdData(struct ThrdData *thrd_data);

void
InitThrdData(struct ThrdData *thrd_data)
{
  InitializeSRWLock(&thrd_data->mtx);
  InitializeConditionVariable(&thrd_data->reqs_is_ne);
  InitializeConditionVariable(&thrd_data->wrk_thrds_are_ready);
  InitializeConditionVariable(&thrd_data->wrk_thrds_killed);
  InitReqQueue(&thrd_data->req_queue_head, &thrd_data->req_queue_tail);
}

int
main(void)
{
  struct ThrdData thrd_data;
  TP_CALLBACK_ENVIRON tp_clbk_env;
  TP_CLEANUP_GROUP *tp_cln_grp;
  TP_POOL *tp;
  TP_WORK *main_thrd;

  tp = CreateThreadpool(NULL);
  if (NULL != tp)
  {
    if (SetThreadpoolThreadMinimum(tp, kNumberOfThreads + 1))
    {
      InitializeThreadpoolEnvironment(&tp_clbk_env);
      SetThreadpoolCallbackPool(&tp_clbk_env, tp);
      tp_cln_grp = CreateThreadpoolCleanupGroup();
      if (NULL != tp_cln_grp)
      {
        SetThreadpoolCallbackCleanupGroup(&tp_clbk_env, tp_cln_grp, NULL);
        InitThrdData(&thrd_data);
        thrd_data.tp_clbk_env = &tp_clbk_env;
        thrd_data.end_program = CreateEventW(NULL, TRUE, FALSE, NULL);
        main_thrd = CreateThreadpoolWork(MainThrdCallback, &thrd_data,
            &tp_clbk_env);
        if (NULL != main_thrd)
        {
          SubmitThreadpoolWork(main_thrd);
          WaitForSingleObject(thrd_data.end_program, INFINITE);
        }
        CloseThreadpoolCleanupGroupMembers(tp_cln_grp, TRUE, NULL);
        CloseThreadpoolCleanupGroup(tp_cln_grp);
      }
      DestroyThreadpoolEnvironment(&tp_clbk_env);
    }
    CloseThreadpool(tp);
  }
  return 0;
}
