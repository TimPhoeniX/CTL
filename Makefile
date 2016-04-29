CPPFLAGS=-std=c++11 -Wall -Wextra -O3
CC=g++
Name=wiki.exe
Sources=wiki.cpp
Headers= 

.PHONY: default run clean tar

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS)

run: $(Name)
	./$<

clean:
	rm -rf *~
	rm -rf *.exe
	