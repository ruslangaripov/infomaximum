#ifndef DATA_HXX_
#define DATA_HXX_

#include <deque>
#include <windows.h>
#include "if.hxx"
#include "sync/cond_var.hxx"
#include "sync/srw_lock.hxx"

struct Data
{
  SrwLock mtx;
  CondVar reqs_is_ne;
  CondVar wrk_thrds_are_ready;
  CondVar wrk_thrds_killed;
  std::deque<Request *> reqs;
  TP_CALLBACK_ENVIRON *tp_env;
  void *end_program;
  void *kill;
  int wrk_thrds_run;
};

#endif  /* DATA_HXX_ */
