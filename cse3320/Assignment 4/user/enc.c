#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
  int fd = open("README", 2);
  int key = 13;

  if (encrypt(fd, key) == -1)
  {
    return -1;
  }
  
  return 0;
}