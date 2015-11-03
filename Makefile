CPPFLAGS= -std=c++11 -Wall -Wextra
CC = g++
Name = test.exe
Sources= test.cpp
Headers= ctl_data.hpp ctl_sort.hpp
files= $(Sources) ctl_data.hpp Makefile dane.txt wyniki_stable.txt  

.PHONY: default run clean tar

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS)

run: $(Name)
	./$<

clean:
	rm -rf *~
	rm -rf *.exe