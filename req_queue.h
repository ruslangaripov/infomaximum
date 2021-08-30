#ifndef REQ_QUEUE_H_
#define REQ_QUEUE_H_

#include "if.h"

struct ReqQueueData
{
  struct Request *req;
  struct ReqQueueData *next;
};

extern void ClearReqQueue(struct ReqQueueData **head,
    struct ReqQueueData **tail);
extern struct Request *DequeueReq(struct ReqQueueData **head);
extern void EnqueueReq(struct ReqQueueData **head, struct ReqQueueData **tail,
    struct Request *req);
extern void InitReqQueue(struct ReqQueueData **head,
    struct ReqQueueData **tail);

#endif  /* REQ_QUEUE_H_*/
