CCFLAGS = -Wall -Wshadow -O2 -g
LDLIBS = -lm


all: test example1 example2 example3 example4 example5 nn


test: test.o genann.o

check: test
	./$^

example1: example1.o genann.o

example2: example2.o genann.o

example3: example3.o genann.o

example4: example4.o genann.o

example5: example5.o genann.o

nn: nn.o genann.o

clean:
	$(RM) *.o
	$(RM) *.exe
	$(RM) persist.txt
