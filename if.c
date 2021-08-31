#include "if.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <windows.h>
#include <bcrypt.h>

static const size_t kReqNmbr = 10;

static struct Request *req_pool;

static struct Request *GetNextReq(void);
static int GetRandomDelay(int min, int max);

/* The GetRequest() function is called by single main thread only.  The
 * ProcessRequest() one -- by several worker threads.  Both functions imitate
 * data processing by suspending a thread for random time span.  To force a
 * situation when there are no more requests the main thread can get, but the
 * worker threads still have work to do (request queue is not empty),
 * "synthetic" delays in the main thread are much shorter then in the worker
 * threads. */
void
FiniRequests(void)
{
  if (NULL != req_pool)
  {
    /* It's up to caller to release request objects. */
    free(req_pool);
    req_pool = NULL;
  }
}

struct Request *GetNextReq(void)
{
  static size_t idx = 0;

  return kReqNmbr > idx ? *((struct Request **)req_pool + idx++) : NULL;
}

int
GetRandomDelay(int min, int max)
{
  BCRYPT_ALG_HANDLE cng_algo;
  int result;

  result = 0;
  if (STATUS_SUCCESS == BCryptOpenAlgorithmProvider(&cng_algo,
      BCRYPT_RNG_ALGORITHM, NULL, 0))
  {
    while (STATUS_SUCCESS == BCryptGenRandom(cng_algo, (unsigned char *)&result,
        sizeof result, 0))
    {
      result %= max;
      if (min < result)
      {
        break;
      }
    }
    BCryptCloseAlgorithmProvider(cng_algo, 0);
  }
  return result;
}

struct Request *
GetRequest(void)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  struct Request *req;
  int sleep;

  req = GetNextReq();
  if (NULL != req)
  {
    sleep = GetRandomDelay(10, 100);
    printf("[%ld] Main thread imitates creation of request #%d for %d ms.\n",
        thrd_id, req->id, sleep);
    Sleep(sleep);
  }
  return req;
}

void
InitRequests(void)
{
  struct Request *req;
  size_t i;

  req_pool = malloc(kReqNmbr * sizeof req_pool);
  if (NULL != req_pool)
  {
    memset(req_pool, 0, kReqNmbr * sizeof req_pool);
    for (i = 0; kReqNmbr > i; ++i)
    {
      req = malloc(sizeof *req);
      if (NULL != req)
      {
        req->id = (int)i;
        *((struct Request **)req_pool + i) = req;
      }
      else
      {
        break;
      }
    }
  }
}

void
ProcessRequest(const struct Request *request)
{
  const unsigned long int thrd_id = GetCurrentThreadId();

  int sleep;

  sleep = GetRandomDelay(100, 1000);
  if (NULL != request)
  {
    printf("[%ld] Worker thread imitates processing of request #%d for %d "
        "ms.\n", thrd_id, request->id, sleep);
  }
  else
  {
    printf("[%ld] Worker thread imitates processing of request for %d ms.\n",
        thrd_id, sleep);
  }
  Sleep(sleep);
}
