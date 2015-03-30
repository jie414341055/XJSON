# compiler name
CC=g++

# settings
CFLAGS=-c

# files
SOURCES=../XJSON/JSON.cpp ../XJSON/JSONValue.cpp
HEADERS=../XJSON/JSON.h ../XJSON/JSONValue.h
OBJECTS=JSON.o JSONValue.o

demo1 demo2: $(OBJECTS)
	$(CC) demo1.cpp $(OBJECTS) -o examp1
	$(CC) demo2.cpp $(OBJECTS) -o examp2

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES)

