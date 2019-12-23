WARNING = -Wall -Wshadow --pedantic
VAL = valgrind --leak-check=full --tool=memcheck --log-file=memcheck.txt --show-leak-kinds=all --track-origins=yes --verbose 
ERROR = -Wvla -Werror
HEADERS = huffman.h
SRCS = compress.c
EXE = compress
OBJS = $(SRCS:%.c=%.o)
GCC = cc -std=c99 -g $(WARNING) $(ERROR)
$(EXE): $(OBJS)
	$(GCC) $(OBJS) -o $(EXE)
test: $(EXE)
	./$(EXE) test.in test.out
testmemory: $(EXE)
	$(VAL) ./$(EXE) test.in test.out
	vim memcheck.txt
.c.o:
	$(GCC) -c $*.c
clean:
	rm -rf $(EXE) $(OBJS) memcheck.txt
