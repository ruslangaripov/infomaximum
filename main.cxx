#include <thread>
#include "if.hxx"
#include "main_thrd.hxx"

int
main(void)
{
  std::thread main_thrd(MainThrd, kNumberOfThreads);

  main_thrd.join();
  return 0;
}
