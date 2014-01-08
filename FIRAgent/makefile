CC = g++
CFLAGS = -g `mysql_config --cflags`
LIBS = -lwiringPi `mysql_config --libs`

TempMonitor: TempMonitor.o BMP085.o
	$(CC) $(LIBS) -o TempMonitor TempMonitor.o BMP085.o

TempMonitor.o: TempMonitor.cpp 
	$(CC) $(CFLAGS) -c TempMonitor.cpp

BMP085.o: BMP085.cpp BMP085.h
	$(CC) $(CFLAGS) -c BMP085.cpp

TempMonitor.cpp:

BMP085.cpp:

BMP085.h:

clean:
	rm TempMonitor.o BMP085.o TempMonitor
