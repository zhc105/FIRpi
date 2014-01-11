#include "MyLock.h"

MyLock::MyLock()
{
    sem_init(&sem, 0, 1);
}

MyLock::~MyLock()
{
    sem_destroy(&sem);
}

bool MyLock::Lock()
{
    if ( sem_wait(&sem) != 0 )
        return false;
    return true;
}

bool MyLock::Unlock()
{
    if ( sem_post(&sem) != 0 )
        return false;
    return true;
}
