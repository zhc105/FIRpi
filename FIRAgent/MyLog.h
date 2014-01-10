#ifndef _MYLOG_H_
#define _MYLOG_H_

#include <cstdio>

class MyLog
{
public:
	static const int LOG_INFO 	= 0;
	static const int LOG_WARNING	= 1;
	static const int LOG_ERROR	= 2;

private:
	static const char *FILENAME;
	static FILE *LogFile;

public:
	static void Init();
	static void WriteLog(const char *msg, int type);

};

#endif
