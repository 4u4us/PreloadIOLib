//#define _GNU_SOURCE
#include<sys/stat.h>
#include<sys/types.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

ssize_t (*orig_write)(int , const void *, size_t ) = NULL;

void printDebug(char* printThis)
{
	(*orig_write)(1,printThis,strlen(printThis));
}

extern "C" ssize_t write(int fd, const void *buf, size_t count)
{
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");

	char wrbuf[PATH_MAX];
	char lnkbuf[PATH_MAX];
    snprintf(wrbuf,PATH_MAX,"/proc/%d/fd/%d",getpid(),fd);
	readlink(wrbuf, lnkbuf, PATH_MAX);
	int temp=strlen(lnkbuf);
	lnkbuf[temp]='\n';
	lnkbuf[temp+1]=0;
	
	char blackListedStr[]="hsperfdata";
	if (NULL==strstr(lnkbuf, blackListedStr))
	{
		printDebug(lnkbuf);
		return (*orig_write)(fd, buf, count);
	}
	else
	{
		return 0;	
	}
	

}









