# modify TARGET to be either C file to compile properly
CFLAGS = -Wextra -Wall -pedantic
TARGET = graphics

$(TARGET): $(TARGET).c
	cc $(TARGET).c $(CFLAGS) -o $(TARGET)	

run:
	./$(TARGET)

view:
	feh .

video:
	ffmpeg -i output-%02d.ppm -r 60 output.mp4
	@echo "video generated!"

clean:
	rm -f $(TARGET)
	rm -f *.ppm

help:
	@echo "Available make targets:"
	@echo "...run:   generate all .ppm images"
	@echo "...view:  view all .ppm images in the pwd"
	@echo "...video: stitch together all .ppm images into one video"
	@echo "...clean: remove the compiled binary and all .ppm images"
