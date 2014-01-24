CC = g++
CFLAGS = -O2 -g
LIBS = -lm -lpthread
SOURCES =	IFIRAgent.cpp \
		FIRAgent.cpp \
		DoublePlayer.cpp \
		FIRDaemon.cpp \
		MyLock.cpp \
		MyLog.cpp \
		main.cpp

OBJS = 	IFIRAgent.o \
	FIRAgent.o \
	DoublePlayer.o \
	FIRDaemon.o \
	MyLock.o \
	MyLog.o \
	main.o

TARGET = FIRDaemon

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $(TARGET) $(LIBS) 

IFIRAgent.o: IFIRAgent.cpp IFIRAgent.h
	$(CC) $(CFLAGS) -c IFIRAgent.cpp

FIRAgent.o: FIRAgent.cpp FIRAgent.h
	$(CC) $(CFLAGS) -c FIRAgent.cpp

DoublePlayer.o: DoublePlayer.cpp DoublePlayer.h
	$(CC) $(CFLAGS) -c DoublePlayer.cpp

FIRDaemon.o: FIRDaemon.cpp FIRDaemon.h
	$(CC) $(CFLAGS) -c FIRDaemon.cpp

MyLock.o: MyLock.cpp MyLock.h
	$(CC) $(CFLAGS) -c MyLock.cpp

MyLog.o: MyLog.cpp MyLog.h
	$(CC) $(CFLAGS) -c MyLog.cpp

main.o:	main.cpp
	$(CC) $(CFLAGS) -c main.cpp

IFIRAgent.cpp:

IFIRAgent.h:

FIRAgent.cpp:

FIRAgent.h:

DoublePlayer.cpp:

DoublePlayer.h:

FIRDaemon.cpp:

FIRDaemon.h:

MyLock.cpp:

MyLock.h:

MyLog.cpp:

MyLog.h:

main.cpp:

clean:
	rm $(TARGET) $(OBJS)
