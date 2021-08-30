#include "req_queue.h"
#include <stdlib.h>

void
ClearReqQueue(struct ReqQueueData **head, struct ReqQueueData **tail)
{
  struct ReqQueueData *curr, *next;

  curr = *head;
  while (NULL != curr)
  {
    next = curr->next;
    if (NULL != curr->req)
    {
      free(curr->req);
    }
    free(curr);
    curr = next;
  }
  InitReqQueue(head, tail);
}

struct Request *
DequeueReq(struct ReqQueueData **head)
{
  struct ReqQueueData *next_head;
  struct Request *req;

  req = (*head)->req;
  next_head = (*head)->next;
  free(*head);
  *head = next_head;
  return req;
}

void
EnqueueReq(struct ReqQueueData **head, struct ReqQueueData **tail,
    struct Request *req)
{
  struct ReqQueueData *obj;

  obj = malloc(sizeof *obj);
  if (NULL != obj)
  {
    obj->req = req;
    obj->next = NULL;
    if (NULL == *head)
    {
      *head = obj;
    }
    else
    {
      (*tail)->next = obj;
    }
    *tail = obj;
  }
}

void
InitReqQueue(struct ReqQueueData **head, struct ReqQueueData **tail)
{
  *head = NULL;
  *tail = NULL;
}
