#include "ioredirect.h"
#include <unistd.h>

void iredirect(char *ifile)
{
  if(fork() == 0)
  {
    open(ifile, O_RDONLY);
    close(0);
    dup(3);
    close(3);
  }
}

void oredirect(char *ofile)
{
  if(fork() == 0)
  {
    open(ofile, O_RDWR | O_CREAT | O_TRUC);
    close(1);
    dup(3);
    close(3);
  }
}
