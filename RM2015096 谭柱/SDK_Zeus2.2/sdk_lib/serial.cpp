#include "serial.h"
#include <unistd.h>

void serial(void)
{
   unsigned char *buf, i=0;
   buf =&i;
   *buf= 0102;
   int len = 3;
   write(16,buf,len);
}
