# CFLAGS are just to optimize the program
# SMALLFLAGS aim to make it tinier per gek's challenge (his is 113 bytes?????????????)
# see this for why we use -Os: https://stackoverflow.com/questions/19689014/gcc-difference-between-o3-and-os
# more info here too: https://stackoverflow.com/questions/200292/process-for-reducing-the-size-of-an-executable
TARGET = main
CFLAGS = -O2 -Wextra -Werror -Wall -pedantic
SMALLFLAGS = -Os -s -fno-ident -g0 -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections -fdata-sections -Wl,--strip-all -Wl,--gc-sections -Wl,--build-id=none
# optimize for size, strip binary of all symbols and do dead code elimination, remove gcc propaganda, no debug info, 0 stack protection

$(TARGET): $(TARGET).c
	cc $(CFLAGS) $(TARGET).c -o $(TARGET)

small:	
	cc $(SMALLFLAGS) $(TARGET).c -o $(TARGET)-small
	# fun read as well: https://www.muppetlabs.com/~breadbox/software/tiny/teensy.html (gets a bit insane as u go on lol)

clean:
	rm -f $(TARGET) $(TARGET)-small
