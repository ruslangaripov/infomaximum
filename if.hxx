#ifndef IF_HXX_
#define IF_HXX_

class Request
{
public:
  explicit Request(int id)
  {
    m_id = id;
  }

  int
  GetId(void) const
  {
    return m_id;
  }

private:
  int m_id;
};

/*
Request *GetRequest() throw();
void ProcessRequest(Request *request) throw();

const int NumberOfThreads = 2;
*/

Request *GetRequest(void) noexcept(true);
void ProcessRequest(const Request *request) noexcept(true);

const int kNumberOfThreads = 2;

#endif  /* not IF_HXX_ */
