#include "MyLog.h"

const char * MyLog::FILENAME = "server.log";
FILE *MyLog::LogFile = NULL;

void MyLog::Init()
{
	LogFile = fopen(FILENAME, "a");
	setvbuf(LogFile, NULL, _IOLBF, 512); 
}

void MyLog::WriteLog(const char *msg, int type)
{
	switch (type)
	{
	case LOG_INFO:
		fprintf(LogFile, "[LOG] %s\n", msg);
		break;
	case LOG_WARNING:
		fprintf(LogFile, "[WRN] %s\n", msg);
		break;
	case LOG_ERROR:
		fprintf(LogFile, "[ERR] %s\n", msg);
		break;
	}
}
