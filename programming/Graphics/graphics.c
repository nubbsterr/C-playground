// learning from Tsoding's video on graphics APIs being irrelevant (basically just software driven rendering/graphics): https://www.youtube.com/watch?v=xNX9H_ZkfNE
// run make to compile, make run to generate the ppm files, then make video to stitch them together
// make help for some help dialogue

#include <stdio.h>

int main() {
  // we can make a video out of multiple image files,
  // using ffmpeg; ffmpeg -i output-%02d.ppm -r <framerate> output.mp4

  // allocate a buffer to store the generated name of the files
  char fname[256];
  for (int i = 0; i < 60; i++) {
    // we're basically gonna write special data
    // that will alter how the file LOOKS if we
    // view it in an image viewer
    // .ppm is Portable Pixmap Format; simple image file really
    snprintf(fname, sizeof(fname), "output-%02d.ppm", i);
    char* output = fname;
    FILE* f = fopen(output, "wb");
    fprintf(f, "P6\n"); // pppm magic byte for BINARY format, ASCII is P3: https://en.wikipedia.org/wiki/Netpbm

    // the next print is going to specify the size of our image
    // 16*9 ratio
    int w = 16*60;
    int h = 9*60;
    fprintf(f, "%d %d\n", w, h);

    // max value of colour component
    fprintf(f, "255\n");

    // now we basically write byte triplets for RGB colour, for each pixel in the file
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        if (((x+i)/60 + (y+i)/60) % 2) {
          // draw yellow pixel at an offset of 'i' pixels in the
          // x and y (checkered pattern that goes lower and
          // lower into the image, which we can use this to make a
          // looping video)
          fputc(0xFF, f);
          fputc(0xFF, f);
          fputc(0x00, f);
        } else {
          // draw a blueish pixel instead        
          fputc(0x00, f);
          fputc(0xF0, f);
          fputc(0xFF, f);
        }
      }
    }
    fclose(f);
    printf("Generated %s. View w/ feh %s.\n", output, output);
  }
  return 0;
}
