WARNING = -Wall -Wshadow --pedantic
VAL = valgrind --leak-check=full --tool=memcheck --log-file=memcheck.txt --show-leak-kinds=all --track-origins=yes --verbose 
ERROR = -Wvla -Werror
HEADERS = huffman.h
SRCS1 = compress.c
SRCS2 = decompress.c
EXE1 = compress
EXE2 = decompress
OBJS1 = $(SRCS1:%.c=%.o)
OBJS2 = $(SRCS2:%.c=%.o)
GCC = cc -std=c99 -g $(WARNING) $(ERROR)
run: test
$(EXE1): $(OBJS1)
	$(GCC) $(OBJS1) -o $(EXE1)
$(EXE2): $(OBJS2)
	$(GCC) $(OBJS2) -o $(EXE2)
test: $(EXE1) $(EXE2)
	./$(EXE1) compress.c test.out 
	./$(EXE2) test.out test
	diff test compress.c
testmemory: $(EXE1) $(EXE2)
###	$(VAL) ./$(EXE1) test.in test.out
	$(VAL) ./$(EXE2) test.out test
	vim memcheck.txt
.c.o:
	$(GCC) -c $*.c
clean:
	rm -rf $(EXE1) $(EXE2) $(OBJS1) $(OBJS2) memcheck.txt
