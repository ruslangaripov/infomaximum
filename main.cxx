#include <windows.h>
#include "data.hxx"
#include "main_thrd.hxx"
#include "tp/tp.hxx"
#include "tp/tp_cln_grp.hxx"
#include "tp/tp_env.hxx"

int
main(void)
{
  struct Data data;
  MainThrd main_thrd;
  Tp tp;
  TpEnv tp_env;
  TpClnGrp tp_cln_grp;

  tp.Reset(CreateThreadpool(nullptr));
  if (tp && SetThreadpoolThreadMinimum(tp.Get(), kNumberOfThreads + 1))
  {
    SetThreadpoolCallbackPool(tp_env.Get(), tp.Get());
    tp_cln_grp.Reset(CreateThreadpoolCleanupGroup());
    if (tp_cln_grp)
    {
      SetThreadpoolCallbackCleanupGroup(tp_env.Get(), tp_cln_grp.Get(),
          nullptr);
    }
    data.tp_env = tp_env.Get();
    data.end_program = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    main_thrd.SetThrdData(&data);
    main_thrd.Create(tp_env.Get());
    main_thrd.Submit();
    WaitForSingleObject(data.end_program, INFINITE);
    CloseHandle(data.end_program);
    CloseThreadpoolCleanupGroupMembers(tp_cln_grp.Get(), TRUE, nullptr);
  }
  return 0;
}
