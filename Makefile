CC			:= g++
CFLAGS		:= -std=c++17 -fdiagnostics-color=always -Wall -Wpedantic -g
LIBRARIES	:=

SRC			:= src
INCLUDE		:= lib

all: main.exe

main.exe: $(SRC)/main.cpp $(INCLUDE)/*.h $(SRC)/*.hpp
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

run: clean all
	./main.exe "ressources/tiny_test_set/complete_4_5.gr"

clean:
	rm -f *.exe