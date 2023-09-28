CC     = g++
CFLAGS = -g -Wall
LFLAGS = -lglfw3 -lGLEW -lGLU -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl
IFLAGS = -Iinc/

.PHONY: all clean re run

TGT  = main
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TGT)


$(TGT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(IFLAGS) -o $(TGT)

%.o: %.c
	$(CC) $(CFLAGS) $(LFLAGS) $(IFLAGS) -c $^ -o $@ 

clean:
	@rm -f $(TGT) $(OBJS)
	@echo "clean"

re: clean all

run: all
	./$(TGT)
