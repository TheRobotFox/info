CC= gcc
CFLAGS= -fsanitize=address -g -Wall #-Wpedantic

SRC= 	$(wildcard *.c)\
			$(wildcard List/*.c)\

OBJ= $(SRC:%.c=%.o)

info: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) -f
