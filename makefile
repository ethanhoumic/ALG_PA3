# Compiler and flags
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O3

# Valgrind flags
VALGRIND = valgrind
VALGRIND_FLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

# Default target
all: bin/cb

# Optimized version
bin/cb: cycle_breaking_opt.o main_opt.o lib
	$(CC) $(OPTFLAGS) cycle_breaking_opt.o main_opt.o -ltm_usage -Llib -o bin/cb

main_opt.o: src/main.cpp lib/tm_usage.h src/cycle_breaking.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -Ilib -Isrc -o $@

cycle_breaking_opt.o: src/cycle_breaking.cpp src/cycle_breaking.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -Isrc -o $@

lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<

tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $< -Ilib -o $@

# Valgrind target - example: make valgrind ARGS="input.txt output.txt"
valgrind: bin/cb
	$(VALGRIND) $(VALGRIND_FLAGS) bin/cb $(ARGS)

# Clean up
clean:
	rm -rf *.o lib/*.a lib/*.o bin/*