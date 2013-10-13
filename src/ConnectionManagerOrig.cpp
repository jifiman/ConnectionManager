//============================================================================
// Name        : ConnectionManager.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string>
#include <limits.h>

using namespace std;
#define SMALL_BUFFER	256

enum STATE {NOT_CONNECTED,CONNECTING_ADHOC,CONNECTED_ADHOC,CONNECTING_INFRA,CONNECTED_INFRA};
class ErrorHandler
{
	public:
	ErrorHandler()
	{
		char result [PATH_MAX];
			ssize_t oCount = readlink("/proc/self/exe", result, PATH_MAX );
			string sName(result);
			//size_t oPos = sName.find_last_of('\');

			cout <<result<<endl;
			//openlog(cAname,)
	}
	void Log(char *)
	{

	}
	void Die(char *)
	{

	}

};

void CreateFifo(char* cAfifo)
{
	int iError;
	/* create the FIFO (named pipe) */
	iError = mkfifo(cAfifo, 0666);
	if (iError != 0 && errno != EEXIST)
	{
		cout << "Couldn't create pipe" << endl;
		cout << "ERRNO " << errno << endl;
		cout << strerror(errno) << endl;
		exit (-1);
	}
}
void OpenSyslog(char* cAname)
{

}

int main(int argc, char *argv[])
{
	cout << "Starting " << argv[0] << endl; // prints executable name

	int fd;
	char * cAfifo = "/tmp/connectionManagerNamedPipe";
	char cAbuffer[SMALL_BUFFER];
	ssize_t oRead;

	enum STATE eState = NOT_CONNECTED;
	OpenSyslog(argv[0]);

	/* create the FIFO (named pipe) */
	CreateFifo(cAfifo);
	/*fd = open(cAfifo, O_RDONLY | O_NONBLOCK);
	 if (fd == -1)
	 {
	 return (errno);
	 }
	 oRead = read(fd, cAbuffer, SMALL_BUFFER);
	 printf("ERRNO %d oRead %d\n", errno, oRead);
	 close(fd);*/

	fd = open(cAfifo, O_WRONLY  | O_NONBLOCK);

	if (fd == -1)
	{
		printf("ERRNO %d\n", errno);
		cout << strerror(errno) << endl;

	}


	write(fd, "Hi", sizeof("Hi"));
	close(fd);

	/* remove the FIFO */
	//unlink(cAfifo);
	return 0;
}
