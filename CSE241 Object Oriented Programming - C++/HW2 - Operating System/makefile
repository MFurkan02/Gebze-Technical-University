all: clean compile run

compile: shell.cpp shell.h
	@g++ -o test shell.cpp

run:
	@./test

clean:
	@rm -f *.o
	@rm -f test