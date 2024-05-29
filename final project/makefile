CC = g++
CFLAGS = -g -Wall -O3
OBJS = readfile.o 
BINS = FP

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BINS): main.cpp $(OBJS)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@

do1:
	./$(BINS) 

clean:
	rm -rf $(BINS) $(OBJS) 