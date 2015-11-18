CPPFLAGS= -std=c++11 -Wall -Wextra
CC = g++
Name = test.exe
Sources= test.cpp
Headers= ctl_matrix.hpp  

.PHONY: default run clean tar

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS)

run: $(Name)
	./$<

clean:
	rm -rf *~
	rm -rf *.exe