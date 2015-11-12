CPPFLAGS= -std=c++11 -Wall -Wextra
CC = g++
Name = test.exe
Sources= test.cpp
Headers= ctl_data.hpp ctl_sort.hpp Sort/ctl_merge_sort.hpp Sort/ctl_quick_sort.hpp ctl_pair.hpp
files= $(Sources) ctl_data.hpp Makefile dane.txt wyniki_stable.txt  

.PHONY: default run clean tar

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS)

run: $(Name)
	./$<  < test.txt

clean:
	rm -rf *~
	rm -rf *.exe