CC := g++
CC_FLAGS := -I include -I $(SYSTEMC_HOME)/include -std=c++11 -w
CC_LIBS := -L $(SYSTEMC_HOME)/lib-linux64 -lsystemc
CC_FILES := $(wildcard src/*.cc)
OBJ_FILES := $(addprefix obj/,$(notdir $(CC_FILES:.cc=.o)))

all: main

obj/sknobs.o: src/sknobs.c
	gcc -I include -fPIC -Wall -Werror -c -o $@ $^

main: obj/sknobs.o $(OBJ_FILES)
	$(CC) $(CC_FLAGS) $(CC_LIBS) -o $@ $^
    
obj/%.o: src/%.cc
	$(CC) $(CC_FLAGS) $(CC_LIBS) -c -o $@ $<
    
clean:
	rm  -rf main obj/*.o *.log

.PHONY: all main clean
