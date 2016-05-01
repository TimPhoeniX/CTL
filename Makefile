CPPFLAGS=-std=c++11 -Wall -Wextra -O3
CC=g++
Name=wiki.exe
Sources=wiki.cpp
Headers=CTL/Container/ctl_arraylist.hpp CTL/Graph/Graph.hpp
Extra=Sample
Files=$(Sources) $(Headers) $(Extra)

.PHONY: default run clean tar

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS) -DFASTLOAD

run: $(Name)
	./$<
	
clean:
	rm -rf *~
	rm -rf *.exe
	