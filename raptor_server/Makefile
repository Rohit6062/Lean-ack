CC = gcc 
CFLAGS = -Wall -Wextra  -I. 

#TARGET output 

TARGET = recieve 

SRCS = reciever_test.c ../raptor_header/queue.c ../raptor_header/gf2matrix.c ../raptor_header/raptor_header.c ../raptor_header/raptor_consts.c

OBJS = $(SRCS:.c=.o) 

all:$(TARGET) 
	./$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm

%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(OBJS) $(TARGET)
