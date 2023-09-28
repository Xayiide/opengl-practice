CC     = g++
CFLAGS = -g -Wall
LFLAGS = -lglfw3 -lGLEW -lGLU -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl

.PHONY: all clean re run

TGT  = main
SRCS = main.cpp shader.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TGT)


$(TGT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(TGT)

%.o: %.cpp
	$(CC) $(CFLAGS) $(LFLAGS) -c $^ -o $@ 

clean:
	@rm -f $(TGT) $(OBJS)
	@echo "clean"

re: clean all

run: all
	./$(TGT)
