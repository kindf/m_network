OBJECTS = ./demo.cpp ../EventLoop.cpp ../TimerQueue.cpp ../Timer.cpp ../Timestamp.cpp ../Sockets.cpp ../Epoller.cpp ../Channel.cpp ../InetAddress.cpp ../Acceptor.cpp ../TcpConnection.cpp ../EventLoopThread.cpp ../TcpServer.cpp ../EventLoopThreadPool.cpp ./HttpServer.cpp ./HttpSession.cpp
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