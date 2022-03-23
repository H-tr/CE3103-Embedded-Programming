# Define a variable 'objects'
objects = hello-main.o hello-funct1.o hello-funct2.o
CFLAGS = -c -Wall -g

# Linking rules
all: hello-main.o hello-funct1.o hello-funct2.o
	gcc -pg -o gprof hello-main.o hello-funct1.o hello-funct2.o hello.h

debug: $(objects) hello.h
	gcc -g -Wall -o helloworld-d $(objects) hello.h

hello:	$(objects) hello.h
	gcc -o helloworld $(objects) hello.h

# Compiling rules
hello-main.o: hello-main.c
	gcc $(CFLAGS) hello-main.c

hello-funct2.o: hello-funct2.c
	gcc $(CFLAGS) hello-funct2.c

hello-funct1.o: hello-funct1.c
	gcc $(CFLAGS) hello-funct1.c

clean:
	rm -rf $(objects) helloworld

clean1:
	rm -rf $(DEBUG) helloworld-d
