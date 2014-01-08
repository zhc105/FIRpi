#include <unistd.h>
#include "FIRDaemon.h"

FIRDaemon::FIRDaemon()
{
	Busy = false;
	Agent = NULL;
}

int FIRDaemon::Start()
{
	daemon(1, 0);
	sleep(20);
	return 0;
}


