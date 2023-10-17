CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lm
SOURCES=ftpServer.c commands.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=serverFTP

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o ftp serverFTP