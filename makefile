# If you are receiving warnings, please update your GCC
CC=gcc
FLAGS=-Wall -Wextra -pedantic -std=c99

.PHONY: all
all: CastleWhite.exe

CastleWhite.exe: castle.o events.o mybanner.o charset.o
	$(CC) $(FLAGS) $^ -o $@

charset.o: charset.c charset.h
	$(CC) $(FLAGS) -c $<

mybanner.o: mybanner.c mybanner.h
	$(CC) $(FLAGS) -c $<

events.o: events.c castle.h
	$(CC) $(FLAGS) -c $<

castle.o: castle.c
	$(CC) $(FLAGS) -c $<

.PHONY: run
run: CastleWhite.exe
	./$<

.PHONY: clean
clean:
	@rm -rf *~
	@rm -rf *.o
	@rm -rf *.txt
	
.PHONY: debug
debug: CastleWhite.exe
	./$< -debug
	
.PHONY: delete
delete: clean
	@rm -rf *.exe