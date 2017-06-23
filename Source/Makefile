CC = gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -Werror -c#-std=c99 -g -lm
CFLAGS2 = -g -Wall -Wextra -Wpedantic -Werror#-std=c99 -g -lm

.PHONY: all
all: encode decode

encode:	encode.o queue.o stack.o huffman.o treestack.o bv.o
	$(CC) $(CFLAGS2) encode.o queue.o huffman.o stack.o treestack.o bv.o -o encode

encode.o:encode.c
	$(CC) $(CFLAGS) encode.c

decode:	decode.o huffman.o stack.o treestack.o
	$(CC) $(CFLAGS2) decode.o huffman.o stack.o treestack.o -o decode

decode.o: decode.c
	$(CC) $(CFLAGS) decode.c

bv.o: bv.c
	$(CC) $(CFLAGS) bv.c

queue.o: queue.c
	$(CC) $(CFLAGS) queue.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) huffman.c

stack.o: stack.c
	$(CC) $(CFLAGS) stack.c

treestack.o: treestack.c
	$(CC) $(CFLAGS) treestack.c

.PHONY: clean
clean:
	rm -f *.o decode encode

