#include "FIRDaemon.h"
#include "MyLog.h"

int main()
{
        FIRDaemon server;
	MyLog::Init();

	return server.Start();
}

