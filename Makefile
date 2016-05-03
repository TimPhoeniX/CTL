CPPFLAGS=-std=c++11 -Wall -Wextra -O3
CC=g++
Name=wiki.exe
Sources=wiki.cpp
Headers=CTL/Container/ctl_arraylist.hpp CTL/Graph/Graph.hpp CTL/ctl_pair.hpp CTL/Container/ctl_stack.hpp CTL/Utility/ctl_traits.hpp
Extra=Sample.txt SampleResult.txt Makefile
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
	./$< < Sample.txt > SampleResult.txt
	
tar: $(Files)
	mkdir Wiki
	cp --parent $(Files) Wiki
	tar -czvf AiSD2-2-PiotrGrudzien.tar.gz Wiki
	rm -rf Wiki
