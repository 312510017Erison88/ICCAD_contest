CC = g++
CFLAGS = -g -Wall -O3
OBJS = readfile.o BFS.o
BINS = FP

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BINS): main.cpp $(OBJS)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@

do1:
	./$(BINS) case4/case4_cfg.json case4/case4_small.json case4/case4_def/chip_top.def case4/case4_def

clean:
	rm -rf $(BINS) $(OBJS) *.csv