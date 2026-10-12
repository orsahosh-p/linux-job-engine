CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
CPPFLAGS = -Iinclude

ALL_GCC = $(CC) $(CFLAGS) $(CPPFLAGS)

TARGET = build/job_engine
OBJECTS = build/main.o build/job.o build/job_vector.o build/job_queue.o build/process.o build/job_heap.o build/scheduler.o

.PHONY: all run clean

all: $(TARGET)

build/main.o: src/main.c include/job.h include/job_vector.h include/job_queue.h include/process.h include/job_heap.h
	$(ALL_GCC) -c $< -o $@

build/job.o: src/job.c include/job.h
	$(ALL_GCC) -c $< -o $@

build/job_vector.o: src/job_vector.c include/job_vector.h include/job.h
	$(ALL_GCC) -c $< -o $@

build/job_queue.o: src/job_queue.c include/job_queue.h include/job.h
	$(ALL_GCC) -c $< -o $@

build/process.o: src/process.c include/process.h
	$(ALL_GCC) -c $< -o $@

build/job_heap.o: src/job_heap.c include/job_heap.h
	$(ALL_GCC) -c $< -o $@

build/scheduler.o: src/scheduler.c include/scheduler.h
	$(ALL_GCC) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(ALL_GCC) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
