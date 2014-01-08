CC = g++
CFLAGS = -g
LIBS = -lm -lpthread
SOURCES = IFIRAgent.cpp FIRAgent.cpp FIRDaemon.cpp main.cpp
OBJS = IFIRAgent.o FIRAgent.o FIRDaemon.o main.o
TARGET = FIRDaemon

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $(TARGET) $(LIBS) 

IFIRAgent.o: IFIRAgent.cpp IFIRAgent.h
	$(CC) $(CFLAGS) -c IFIRAgent.cpp

FIRAgent.o: FIRAgent.cpp FIRAgent.h
	$(CC) $(CFLAGS) -c FIRAgent.cpp

FIRDaemon.o: FIRDaemon.cpp FIRDaemon.h
	$(CC) $(CFLAGS) -c FIRDaemon.cpp

main.o:	main.cpp
	$(CC) $(CFLAGS) -c main.cpp

IFIRAgent.cpp:

IFIRAgent.h:

FIRAgent.cpp:

FIRAgent.h:

FIRDaemon.cpp:

FIRDaemon.h:

main.cpp:

clean:
	rm $(TARGET) $(OBJS)
