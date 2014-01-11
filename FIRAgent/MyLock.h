#ifndef _MYLOCK_H_
#define _MYLOCK_H_

#include <semaphore.h>

class MyLock
{
private:
	sem_t sem;

public:
	MyLock();
	~MyLock();
	bool Lock();
	bool Unlock();

};

#endif
