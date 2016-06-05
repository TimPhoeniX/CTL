CPPFLAGS=-std=c++11 -Wall -Wextra -O2
CC=g++
Name=Maze.exe
Sources=main.cpp
Headers=
Extra=Makefile
Files=$(Sources) $(Headers) $(Extra)

.PHONY: default run clean tar Sample

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS)

run: $(Name)
	./$<
	
clean:
	rm -rf *~
	rm -rf *.exe