CC = g++
CFLAGS = -g -Wall -O3
OBJS = readfile.o Greedy.o
BINS = CGR

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BINS): main.cpp $(OBJS)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@

do1:
	./$(BINS) 10 case4/case4_def case4/case4_cfg.json case4/case4_small.json

clean:
	rm -rf $(BINS) $(OBJS) *.csv