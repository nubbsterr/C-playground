TARGET = main
CFLAGS = -O2 -Wno-unused-parameter -Wno-unused-but-set-variable -Wextra -Wall -pedantic -Werror
DEBUGFLAGS = -g -Wextra -Wall -pedantic -Wno-unused-parameter -Wno-unused-but-set-variable -Werror
$(TARGET): $(TARGET).c
	cc $(CFLAGS) $(TARGET).c -o $(TARGET)

# exclusively for using valgrind
debug:
	cc $(DEBUGFLAGS) $(TARGET).c -o $(TARGET)-dbg
	valgrind ./$(TARGET)-dbg

clean:
	rm -f $(TARGET) $(TARGET)-dbg
