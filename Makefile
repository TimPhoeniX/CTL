CPPFLAGS=-std=c++11 -Wall -Wextra
CC=g++
Name=test.exe
Sources=test.cpp
Headers=
Extra=Makefile
Files=$(Sources) $(Headers) $(Extra)

.PHONY: default run clean tar Sample

default: $(Name)

$(Name): $(Sources) $(Headers)
	$(CC) -o $@ $(Sources) $(CPPFLAGS) -DFASTLOAD

run: $(Name)
	./$< < test.txt > mineout.txt
	
clean:
	rm -rf *~
	rm -rf *.exe

tar: $(Files)
	mkdir Test
	cp --parent $(Files) Wiki
	tar -czvf AiSD2-2-PiotrGrudzien.tar.gz Wiki
	rm -rf Wiki
