CPPFLAGS=-std=c++11 -Wall -Wextra -O3
CC=g++
Name=wiki.exe
Sources=wiki.cpp
Headers=CTL/Container/ctl_arraylist.hpp CTL/Graph/Graph.hpp

.PHONY: default run clean tar Fast

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS) -DFASTLOAD

run: $(Name)
	./$<
	
clean:
	rm -rf *~
	rm -rf *.exe
	