CC= gcc
CFLAGS= -fsanitize=address -g #-Wall #-Wpedantic
#CFLAGS= -Ofast -funsafe-math-optimizations -mavx2#-fsanitize=address -g -Wall #-Wpedantic

SRC= 	$(wildcard *.c)\
			$(wildcard List/*.c)\

OBJ= $(SRC:%.c=%.o)

info: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) -f
