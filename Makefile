CC			:= g++
CFLAGS		:= -std=c++17 -fdiagnostics-color=always -Wall -Wpedantic -g
LIBRARIES	:=

SRC			:= src
INCLUDE		:= lib

all: main.exe

main.exe: $(SRC)/main.cpp $(INCLUDE).h $(INCLUDE).hpp 
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

run: clean all
	./main.exe

clean:
	rm -f *.exe