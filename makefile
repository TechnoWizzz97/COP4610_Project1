CC=gcc
CFLAGS=-I.
DEPS = shell.h
OBJ = shell.o parse.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simpleShell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o simpleShell 
