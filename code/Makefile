CC = g++

CFLAGS = -std=c++0x

SRCS = main.cpp drone/Ardrone.cpp drone/Controller.cpp drone/navigation/State.cpp  drone/navigation/Telemetry.cpp core/comm/UdpSocket.cpp core/time/TimeTools.cpp

OBJS = $(SRCS:.c=.o)

INCLUDES = -I.

LIBS = -lpthread -lm

MAIN = output

.PHONY: depend clean

all:	$(MAIN)
	@echo Simple compiler named output has been compiled

$(MAIN):
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(SRCS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
