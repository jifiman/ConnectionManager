#include <string>


class Logger
{
    public:
    void LogMessage(std::string sMessage)
    {
    //log the message
    }
    Logger()
    {
    	//initialize Logger
    }
    
} myLogger;


class Alpha
{

    public:
    void Func(void)
    {
        myLogger.LogMessage("func");
    }

};


int main2(int argc, char *argv[])
{
	Alpha myAlpha;
	myAlpha.Func();
}