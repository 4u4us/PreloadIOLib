//#define _GNU_SOURCE
#include<sys/stat.h>
#include<sys/types.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include <libgen.h>

ssize_t (*orig_write)(int , const void *, size_t ) = NULL;
int (*orig_open)(char* , int, mode_t ) = NULL;

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
	memset(wrbuf,0,PATH_MAX);
	memset(lnkbuf,0,PATH_MAX);
    snprintf(wrbuf,PATH_MAX,"/proc/%d/fd/%d",getpid(),fd);
	readlink(wrbuf, lnkbuf, PATH_MAX);
	int temp=strlen(lnkbuf);
	lnkbuf[temp]='\n';
	
	char blackListedStr[]="hsperfdata";
	if (NULL==strstr(lnkbuf, blackListedStr))
	{
		wrbuf[0]='\n';
		wrbuf[1]='L';
		wrbuf[2]='N';
		wrbuf[3]='W';
		wrbuf[4]='\n';
		wrbuf[5]=0;
		printDebug(wrbuf);
		printDebug(lnkbuf);
		return (*orig_write)(fd, buf, count);
	}
	else
	{	
		return 0;	
	}
	

}

extern "C" int open(char* fname, int flags, mode_t aMode)
{
	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig_open == NULL )
        orig_open = (ssize_t (*)(char* , int, mode_t))dlsym(RTLD_NEXT, "open");

	// test on RO dir path of a file  (use dirname)
    char wrbuf[PATH_MAX];
	memset(wrbuf,0,PATH_MAX);
	struct stat fileStat;
	char fnameTest[] = "/proc/sys/fs/file-nr";
	char* ts1 = strdup(fnameTest); // str duplicate, no need to free mem!
	stat( dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='R';
		wrbuf[1]='O';
		wrbuf[2]='\n';
		printDebug(wrbuf);
	}

	wrbuf[0]='\n';
	wrbuf[1]='-';
	wrbuf[2]='-';
	wrbuf[3]='\n';	
	printDebug(wrbuf);
	printDebug(fname);
	wrbuf[0]='\n';
	wrbuf[1]='+';
	wrbuf[2]='+';
	wrbuf[3]='\n';	
    printDebug(wrbuf);
	   
	return (*orig_open)(fname, flags, aMode);
}








