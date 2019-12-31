WARNING = -Wall -Wshadow --pedantic
VAL = valgrind --leak-check=full --tool=memcheck --log-file=memcheck.txt --show-leak-kinds=all --track-origins=yes --verbose 
ERROR = -Wvla -Werror
HEADERS = huffman.h
SRCS1 = huff/compress.c huff/encode.c huff/misc.c huff/queue.c huff/sort.c huff/tree.c huff/read.c
SRCS2 = dehuff/decode.c dehuff/decompress.c dehuff/list.c dehuff/misc.c dehuff/read.c dehuff/tree.c
EXE1 = compress
EXE2 = decompress
OBJS1 = $(SRCS1:%.c=%.o)
OBJS2 = $(SRCS2:%.c=%.o)
GCC = cc -std=c99 -g $(WARNING) $(ERROR)
run: test
$(EXE1): $(OBJS1)
	mv *.o huff
	$(GCC) $(OBJS1) -o $(EXE1)
$(EXE2): $(OBJS2)
	mv *.o dehuff
	$(GCC) $(OBJS2) -o $(EXE2)
test: $(EXE1) $(EXE2)
	./$(EXE1) Makefile test.out 
	./$(EXE2) test.out test
	diff test Makefile
testmemory: $(EXE1) $(EXE2)
	$(VAL) ./$(EXE1) Makefile test.out
	vim memcheck.txt
	$(VAL) ./$(EXE2) test.out test
	vim memcheck.txt
.c.o: $(SRCS1) $(SRCS2)
	$(GCC) -c $*.c
clean:
	rm -rf $(EXE1) $(EXE2) $(OBJS1) $(OBJS2) memcheck.txt
