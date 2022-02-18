# Define a variable 'objects'
objects = hello-main.o hello-funct1.o
CFLAGS = -c -Wall -g

# Linking rules
all: hello

hello:	$(objects)
	gcc -o hello $(objects) hello.h

# Compiling rules
hello-main.o: hello-main.c
	gcc $(CFLAGS) hello-main.c

hello-funct1.o: hello-funct1.c
	gcc $(CFLAGS) hello-funct1.c

clean:
	rm -f $(objects) hello
