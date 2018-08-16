#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
int fd = open( "textLinuxApp.preloadtest", O_CREAT | O_RDWR, 0666 );

char wrBuf[5];
snprintf(wrBuf,sizeof(wrBuf),"HELL");

write(fd,wrBuf,strlen(wrBuf));

close(fd);

return 0;
}
