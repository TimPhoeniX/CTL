CPPFLAGS=-std=c++11 -Wall -Wextra -O3
CC=g++
Name=wiki.exe
Sources=wiki.cpp
Headers=CTL/Container/ctl_arraylist.hpp CTL/Graph/Graph.hpp CTL/ctl_pair.hpp CTL/Container/ctl_stack.hpp CTL/Utility/ctl_traits.hpp
Extra=Sample SampleResult Makefile
Files=$(Sources) $(Headers) $(Extra)

.PHONY: default run clean tar Sample

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS) -DFASTLOAD

run: $(Name)
	./$<
	
clean:
	rm -rf *~
	rm -rf *.exe

sample: $(Name)
	./$< < Sample > SampleResult
	
tar: $(Files)
	tar -czvf grudzienaisd-2.tar.gz $(Files)