#ifndef IF_H_
#define IF_H_

struct Request
{
  int id;
};

/*
Request *GetRequest() throw();
void ProcessRequest(Request *request) throw();

const int NumberOfThreads = 2;
*/

extern struct Request *GetRequest(void);
extern void InitRequests(void);
extern void FiniRequests(void);
extern void ProcessRequest(const struct Request *request);

static const int kNumberOfThreads = 2;

#endif  /* not IF_H_ */
