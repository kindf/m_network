OBJECTS = ./demo.cpp ./HttpServer.cpp ./HttpSession.cpp ../Network/*.cpp ../Tool/StringTool.cpp
CC = g++
CPPFLAGS = -g -o 
# LIBS = -I /usr/lib64/mysql -lmysqlclient -ljson-c
LIBS = -pthread 
OUTPUT = demo

test:$(OBJECTS)
	$(CC) $(CPPFLAGS) $(OUTPUT) $(OBJECTS) $(LIBS)

.PHONY:clean
clean:
	rm $(OBJECTS)
