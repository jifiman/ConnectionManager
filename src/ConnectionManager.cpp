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
#include "boost/statechart/state_machine.hpp"
#include "boost/statechart/event.hpp"
#include "boost/statechart/simple_state.hpp"
#include <iostream>
#include <openssl/sha.h>
#include <curl/curl.h>

#define SMALL_BUFFER	256
#define LARGE_BUFFER	1024

namespace sc = boost::statechart;
struct Greeting;
struct Machine : sc::state_machine<Machine, Greeting>
{
};
struct Greeting : sc::simple_state<Greeting, Machine>
{
	// Whenever the state machine enters a state, it creates an
	// object of the corresponding state class. The object is then
	// kept alive as long as the machine remains in the state.
	// Finally, the object is destroyed when the state machine
	// exits the state. Therefore, a state entry action can be
	// defined by adding a constructor and a state exit action can
	// be defined by adding a destructor.
	Greeting()
	{
		std::cout << "Hello World!\n";
	} // entry
	~Greeting()
	{
		std::cout << "Bye Bye World!\n";
	} // exit
};

enum STATE
{
	NOT_CONNECTED, CONNECTING_ADHOC, CONNECTED_ADHOC, CONNECTING_INFRA, CONNECTED_INFRA
};
class ErrorHandler
{
private:
	std::string gSname;
public:
	ErrorHandler()
	{
		char result[PATH_MAX];
		ssize_t oCount = readlink("/proc/self/exe", result, PATH_MAX);
		std::string sName(result);
		size_t oPos;

		oPos = sName.find_last_of('/');
		gSname = sName.substr(oPos + 1);
		std::cout << gSname << std::endl;
		openlog(gSname.c_str(), LOG_PERROR, LOG_USER);
		syslog(LOG_INFO, "Started");
	}
	void Log(char * cAlog)
	{
		syslog(LOG_INFO, cAlog);
	}
	void Die(char *cAlog)
	{
		this->Log(cAlog);
		exit(-1);
	}

} gOerrorHandler;

void CreateFifo(char* cAfifo)
{

}
void OpenSyslog(char* cAname)
{

}
class Fifo
{
private:
	char * cAfifo = "/tmp/connectionManagerNamedPipe";

public:
	Fifo()
	{
		int iError;
		/* create the FIFO (named pipe) */
		iError = mkfifo(cAfifo, 0666);
		if (iError != 0 && errno != EEXIST)
		{
			std::string sMessage = "Couldn't create pipe ";
			gOerrorHandler.Die((char*) sMessage.c_str());
		}
	}
	std::string Read()
	{
		char cAbuffer[SMALL_BUFFER];
		ssize_t oTotalBytesRead;
		ssize_t oBytesRead;
		int fd = open(cAfifo, O_RDONLY);
		if (fd == -1)
		{
			std::string sMessage = "Can't open for reading ";
			sMessage += cAfifo;
			gOerrorHandler.Die((char*) sMessage.c_str());
		}
		oTotalBytesRead = 0;
		do
		{
			oBytesRead = read(fd, &cAbuffer[oTotalBytesRead], SMALL_BUFFER - 1 - oTotalBytesRead); //-1 because adding 0 at end
			oTotalBytesRead += oBytesRead;
		}
		while (oBytesRead > 0);
		cAbuffer[oTotalBytesRead] = 0;
		close(fd);
		return std::string(cAbuffer);
	}
	void Write(char *cAmessage)
	{
		int fd = open(cAfifo, O_WRONLY);
		if (fd == -1)
		{
			std::string sError = "Can't open for writing ";
			sError += cAfifo;
			gOerrorHandler.Die((char*) sError.c_str());
		}

		write(fd, cAmessage, sizeof(cAmessage));
		close(fd);
	}
};

class Script
{
private:
	std::string sPath;
public:
	Script(char *cApath)
	{
		std::string sFullPath = cApath;
		size_t iFirst = sFullPath.find("ConnectionManager/");
		if (iFirst == std::string::npos)
		{
			gOerrorHandler.Die("Path is wrong");
		}
		iFirst += strlen("ConnectionManager/");
		sPath = sFullPath.substr(0, iFirst);
		std::cout << "Working Directory " << sPath << std::endl;
	}
	void Run(char *cAscript)
	{
		std::string sCommand = sPath + "scripts/";
		sCommand += cAscript;
		system(sCommand.c_str());
	}
};
class External
{
private:
	char cAhash[SMALL_BUFFER];
public:

	External()
	{
		char cAinput[SMALL_BUFFER];
		unsigned char cAhashValue[SMALL_BUFFER];
		memset(cAhash, 0, SMALL_BUFFER);
		memset(cAhashValue, 0, SMALL_BUFFER);

		FILE * iFile = fopen("/sys/class/net/wlan0/address", "r");
		if (iFile != NULL)
		{
			if (fread(cAinput, 1, SMALL_BUFFER, iFile) != 18)
			{
				gOerrorHandler.Die("problem with MAC address");
			}
		}
		else
			gOerrorHandler.Die("Can't read MAC address");

		fclose(iFile);

		if (simpleSHA256(cAinput, strlen(cAinput), cAhashValue))
		{
			int len = strlen((char const *) cAhashValue);
			int iLoop;
			for (iLoop = 0; iLoop < len; iLoop++)
			{
				sprintf(&cAhash[iLoop], "%X", cAhashValue[iLoop]);
			}
			cAhash[iLoop] = 0; //Terminate string
		}
		else
			gOerrorHandler.Die("Can't calculate SHA256");
	}
	bool simpleSHA256(void* input, unsigned long length, unsigned char* md)
	{
		SHA256_CTX context;
		if (!SHA256_Init(&context))
			return false;

		if (!SHA256_Update(&context, (unsigned char*) input, length))
			return false;

		if (!SHA256_Final(md, &context))
			return false;

		return true;
	}
	void Post(void)
	{
		CURL *curl;
		CURLcode res;

		/* In windows, this will init the winsock stuff */
		curl_global_init (CURL_GLOBAL_ALL);

		/* get a curl handle */
		curl = curl_easy_init();
		if (curl)
		{
			/* First set the URL that is about to receive our POST. This URL can
			 just as well be a https:// URL if that is what should receive the
			 data. */
			curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
			/* Now specify the POST data */
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");

			/* Perform the request, res will get the return code */
			res = curl_easy_perform(curl);
			/* Check for errors */
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			/* always cleanup */
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
	}

};

bool simpleSHA256(void* input, unsigned long length, unsigned char* md);

int main(int argc, char *argv[])
{

	std::cout << "Starting " << argv[0] << std::endl; // prints executable name

	Fifo oFifo;
	Script oScript(argv[0]);
	std::string sInput;
	std::string sSsid;
	std::string sPassWord;
	enum STATE eState = NOT_CONNECTED;

	while (true)
	{
		sInput = oFifo.Read();
		oFifo.Write("network");

		sSsid = oFifo.Read();
		oFifo.Write("ssid");

		sPassWord = oFifo.Read();
		oFifo.Write("password");

		std::cout << "Read " << sInput << ":" << sSsid << ":" << sPassWord << std::endl;

		if (sInput.compare("ADHOC") == 0)
		{
			if (eState != CONNECTED_ADHOC)
			{
				oScript.Run("Adhoc.sh");
				//std::cout<<"Finished"<<std::endl;
				eState = CONNECTED_ADHOC;
			}

		}
		else if (sInput.compare("status") == 0)
		{

		}
		else if (sInput.compare("INFRA") == 0)
		{
			if (eState != CONNECTED_INFRA)
			{
				std::string sCommand = "InfraStructure.sh '";
				sCommand += sSsid;
				sCommand += "'  '" + sPassWord;
				sCommand += "'";
				oScript.Run((char*) sCommand.c_str());
				eState = CONNECTED_INFRA;

			}
		}
	}
	return 0;
}

int main1()
{
	Machine myMachine;
	// The machine is not yet running after construction. We start
	// it by calling initiate(). This triggers the construction of
	// the initial state Greeting
	myMachine.initiate();
	// When we leave main(), myMachine is destructed what leads to
	// the destruction of all currently active states.
	return 0;
}
