//#define _GNU_SOURCE
#include<sys/stat.h>
#include<sys/types.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include <libgen.h>
#include <stdarg.h>


ssize_t (*orig_write)(int , const void *, size_t ) = NULL;
ssize_t (*orig_read)(int , void *, size_t ) = NULL;
//int (*orig_open)(char* , int, mode_t ) = NULL;
int (*orig_open)(char* , int, ... ) = NULL;
int (*orig_ioctl)(int d , unsigned long request,...) = NULL;
//int (*orig_ioctl)(int d , unsigned long request, void* ) = NULL;
int (*orig_fcntl)(int fd , int cmd,...) = NULL;
FILE* (*orig_fopen)(const char* , const char* ) = NULL;
//int (*orig_stat)(const char *path, struct stat *buf)=NULL;
int (*orig__xstat)(int ver, const char * path, struct stat * stat_buf)=NULL;

void printDebug(char* printThis)
{
	(*orig_write)(1,printThis,strlen(printThis));
}

extern "C" ssize_t read(int fd, void *buf, size_t count)
{
	if ( orig_read == NULL )
        orig_read = (ssize_t (*)(int , void *, size_t))dlsym(RTLD_NEXT, "read");

	char wrbuf[PATH_MAX];
	char lnkbuf[PATH_MAX];
	memset(wrbuf,0,PATH_MAX);
	memset(lnkbuf,0,PATH_MAX);
    snprintf(wrbuf,PATH_MAX,"/proc/%d/fd/%d",getpid(),fd);
	readlink(wrbuf, lnkbuf, PATH_MAX);
	int temp=strlen(lnkbuf);
	lnkbuf[temp]='\n';
	/*struct stat fileStat;
	char* ts1 = strdup(lnkbuf); // str duplicate, no need to free mem!
	stat( dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='R';
		wrbuf[1]='O';
		wrbuf[2]='\n';
		printDebug(wrbuf);
	}*/
	
	char blackListedStr[]="hsperfdata";
	if (NULL==strstr(lnkbuf, blackListedStr))
	{
		wrbuf[0]='\n';
		wrbuf[1]='L';
		wrbuf[2]='N';
		wrbuf[3]='R';
		wrbuf[4]='\n';
		wrbuf[5]=0;
		//wrbuf[0]=wrbuf[6];
		//wrbuf[1]=wrbuf[7];
		//wrbuf[2]=wrbuf[8];
		//wrbuf[3]=wrbuf[9];
		//wrbuf[4]='\n';
		//wrbuf[5]=0;
		printDebug(wrbuf);
		printDebug(lnkbuf);
		return (*orig_read)(fd, buf, count);
	}
	else
	{	
		return 0;	
	}
	

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
	struct stat fileStat;
	char* ts1 = strdup(lnkbuf); // str duplicate, no need to free mem!
	stat( dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='R';
		wrbuf[1]='O';
		wrbuf[2]='\n';
		printDebug(wrbuf);
	}
	
	char blackListedStr[]="hsperfdata";
	if (NULL==strstr(lnkbuf, blackListedStr))
	{
		wrbuf[0]='\n';
		wrbuf[1]='L';
		wrbuf[2]='N';
		wrbuf[3]='W';
		wrbuf[4]='\n';
		wrbuf[5]=0;
		//wrbuf[0]=wrbuf[6];
		//wrbuf[1]=wrbuf[7];
		//wrbuf[2]=wrbuf[8];
		//wrbuf[3]=wrbuf[9];
		//wrbuf[4]='\n';
		//wrbuf[5]=0;
		printDebug(wrbuf);
		printDebug(lnkbuf);
		return (*orig_write)(fd, buf, count);
	}
	else
	{	
		return 0;	
	}
	

}

extern "C" int open(char* fname, int flags, ...)
{
	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig_open == NULL )
        orig_open = (ssize_t (*)(char* , int, ...))dlsym(RTLD_NEXT, "open");

	// test on RO dir path of a file  (use dirname)
    char wrbuf[PATH_MAX];
	memset(wrbuf,0,PATH_MAX);
	/*struct stat fileStat;
	char fnameTest[] = "/proc/sys/fs/file-nr";
	char* ts1 = strdup(fnameTest); // str duplicate, no need to free mem!
	stat( dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='R';
		wrbuf[1]='O';
		wrbuf[2]='\n';
		printDebug(wrbuf);
	}*/

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
    
    mode_t a;
    va_list ap;
    va_start(ap, flags);
    a = va_arg(ap, mode_t);
    va_end(ap);
    if (a!=0)
    	return (*orig_open)(fname, flags, a );
    else
    	return (*orig_open)(fname, flags );

}
/*extern "C" int open(char* fname, int flags, mode_t aMode)
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
}*/

extern "C" int fcntl(int fd, int cmd, ...)
{
	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig_fcntl == NULL )
        orig_fcntl = (int (*)(int , int, ...))dlsym(RTLD_NEXT, "fcntl");

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
		wrbuf[3]='F';
		wrbuf[4]='\n';
		wrbuf[5]=0;
		//wrbuf[0]=wrbuf[6];
		//wrbuf[1]=wrbuf[7];
		//wrbuf[2]=wrbuf[8];
		//wrbuf[3]=wrbuf[9];
		//wrbuf[4]='\n';
		//wrbuf[5]=0;
		printDebug(wrbuf);
		printDebug(lnkbuf);
		int a;
		va_list ap;
		va_start(ap, cmd);
		a = va_arg(ap, int);
		va_end(ap);
		if (a!=0)
			return (*orig_fcntl)(fd, cmd, a );
		else
			return (*orig_fcntl)(fd, cmd );
	}
	else
	{	
		return 0;	
	}    
}

extern "C" int ioctl(int fd, unsigned long request, ...)
{
	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig_ioctl == NULL )
        orig_ioctl = (int (*)(int , unsigned long, ...))dlsym(RTLD_NEXT, "ioctl");

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
		wrbuf[3]='I';
		wrbuf[4]='\n';
		wrbuf[5]=0;
		//wrbuf[0]=wrbuf[6];
		//wrbuf[1]=wrbuf[7];
		//wrbuf[2]=wrbuf[8];
		//wrbuf[3]=wrbuf[9];
		//wrbuf[4]='\n';
		//wrbuf[5]=0;
		printDebug(wrbuf);
		printDebug(lnkbuf);
		void* a;
		va_list ap;
		va_start(ap, request);
		a = va_arg(ap, void*);
		va_end(ap);
		if (a!=0)
			return (*orig_ioctl)(fd, request, a );
		else
			return (*orig_ioctl)(fd, request );
	}
	else
	{	
		return 0;	
	}    
}


extern "C" FILE *fopen(const char *pathname, const char *mode)
{
	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig_fopen == NULL )
        orig_fopen = (FILE* (*)(const char* ,const char *))dlsym(RTLD_NEXT, "fopen");

	// test on RO dir path of a file  (use dirname)
    char wrbuf[PATH_MAX];
	memset(wrbuf,0,PATH_MAX);
	/*struct stat fileStat;
	char fnameTest[] = "/proc/sys/fs/file-nr";
	char* ts1 = strdup(fnameTest); // str duplicate, no need to free mem!
	stat( dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='R';
		wrbuf[1]='O';
		wrbuf[2]='\n';
		printDebug(wrbuf);
	}*/

	wrbuf[0]='\n';
	wrbuf[1]='N';
	wrbuf[2]='O';
	wrbuf[3]='\n';	
	printDebug(wrbuf);
	printDebug((char*)pathname);
	wrbuf[0]='\n';
	wrbuf[1]='O';
	wrbuf[2]='N';
	wrbuf[3]='\n';	
    printDebug(wrbuf);
	   
	return (*orig_fopen)(pathname, mode);
}

#if 0
extern "C" int stat(const char *path, struct stat *buf)
{
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig_stat == NULL )
        orig_stat = (int (*)(const char* ,struct stat *))dlsym(RTLD_NEXT, "stat");

	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	// test on RO dir path of a file  (use dirname)
	char wrbuf[PATH_MAX];
	memset(wrbuf,0,PATH_MAX);
	struct stat fileStat;
	char* ts1 = strdup(path); // str duplicate, no need to free mem!
	//if (orig_stat)
	//	(*orig_stat)( dirname(ts1), &fileStat); // dirname in #include <libgen.h>
	//else
	//	stat( dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='S';
		wrbuf[1]='U';
		wrbuf[2]='\n';
		printDebug(wrbuf);
	}

	wrbuf[0]='\n';
	wrbuf[1]='S';
	wrbuf[2]='T';
	wrbuf[3]='\n';	
	printDebug(wrbuf);
	if(path) printDebug((char*)path);
	wrbuf[0]='\n';
	wrbuf[1]='T';
	wrbuf[2]='S';
	wrbuf[3]='\n';	
    printDebug(wrbuf);
	/*if(orig_stat)
		return (*orig_stat)(path, buf);
	else
		return 0;*/
    return 1;
}
#endif


extern "C" int __xstat(int ver, const char * path, struct stat * stat_buf)
{
	if ( orig_write == NULL )
        orig_write = (ssize_t (*)(int , const void *, size_t))dlsym(RTLD_NEXT, "write");
	if ( orig__xstat == NULL )
        orig__xstat = (int (*)(int, const char *, struct stat *))dlsym(RTLD_NEXT, "__xstat");

	// !! Seems that a java app won't call this function on file open
	// inside the app.
	// On the other hand, java.exe seems to call open, e.g to open the jar file
	// to be executed.
	
	// test on RO dir path of a file  (use dirname)
	char wrbuf[PATH_MAX];
	memset(wrbuf,0,PATH_MAX);
	
	wrbuf[0]='\n';
	wrbuf[1]='S';
	wrbuf[2]='T';
	wrbuf[3]='\n';	
	printDebug(wrbuf);
	if(path) printDebug((char*)path);
	wrbuf[0]='\n';
	wrbuf[1]='T';
	wrbuf[2]='S';
	wrbuf[3]='\n';	
    printDebug(wrbuf);	
	
	struct stat fileStat;
	char* ts1 = strdup(path); // str duplicate, no need to free mem!
	(*orig__xstat)(3, dirname(ts1), &fileStat); // dirname in #include <libgen.h>

	if ( (fileStat.st_mode & S_IWUSR) == 0 &&  (fileStat.st_mode & S_IRUSR) )
	{
		wrbuf[0]='\n';
		wrbuf[1]='S';
		wrbuf[2]='U';
		wrbuf[3]='\n';
		printDebug(wrbuf);
	}else
	if (fileStat.st_mode & S_IWUSR)
	{
		wrbuf[0]='\n';
		wrbuf[1]='S';
		wrbuf[2]='W';
		wrbuf[3]='\n';
		printDebug(wrbuf);
	}

    return (*orig__xstat)(ver, path, stat_buf);

}



