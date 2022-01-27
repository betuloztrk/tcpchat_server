CC = gcc
CFLAGS = -Wall -g 
TARGET = serverapp
PWD = $(shell pwd)

SRCS = 	$(PWD)/serverapp.c\
		$(PWD)/msgprotocol/msgprotocol.c\
		$(PWD)/sendrecv/sendrecv.c\
		$(PWD)/userbase/userbase.c\
		$(PWD)/grouptable/grouptable.c\
		$(PWD)/usergroup/usergroup.c\

OBJS = $(SRCS:.c=.o)

INCLUDES = -I/$(PWD)/msgprotocol/\
           -I/$(PWD)/sendrecv/\
           -I/$(PWD)/userbase/\
           -I/$(PWD)/grouptable/\
           -I/$(PWD)/usergroup/
           		   

all: clean $(TARGET) 
	@echo  Simple compiler named serverapp has been compiled
	
$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) -lpthread
	
clean:
	$(RM) serverapp *.o *~ $(OBJS)