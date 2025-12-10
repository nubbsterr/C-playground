#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// use `time <binary>` to benchmark
int main() {
  FILE* in = fopen("./lockdoc", "r");
  if (in == NULL) {
    fprintf(stderr, "Failed to open ./lockdoc!\n");
    return 1;
  }

  char buf[16];     // lines are ~4 characters long max from what i saw
  char rotmode;     // either L or R, will change which way we rotate the dial
  int rot = 0;      // unknown count of numbers after the mode to rotate by
  int password = 0; // increment every time the dial equals 0
  int atzerobeforerot = 0; // special flag raised if dial = 0 at end of a loop, to prevent double count
  int dial = 50;
  // read line by line until we reach
  // the end of the document
  while(fgets(buf, sizeof(buf), in) != NULL) {
      rotmode = buf[0]; // this is always true
      int count = 0;
      while (buf[count] != '\n') {
        count++;
      }
      // modify the remaining rotation substring into an integer
      char* rotstr = malloc(count-1);
      strncpy(rotstr, buf+1, count-1);
      rot = atoi(rotstr);
      switch (rotmode) {
        case 'L':
          while (rot != 0) {
            dial--;
            rot--;
            if (dial < 0) {
              if (!atzerobeforerot && rot != 0)
                password++;
              dial = 99;
            }
            // reset flag after the first iteration
            atzerobeforerot = 0;
          }
          break;
        case 'R':
          while (rot != 0) {
            dial++;
            rot--;
            if (dial > 99) {
              if (!atzerobeforerot && rot != 0)
                password++;
              dial = 0;
            }
            // reset flag after the first iteration
            atzerobeforerot = 0;
          }
          break;
      }

      if (dial == 0) {
        password++;
        atzerobeforerot = 1;
      }

      // free malloc at the end of the iteration
      free(rotstr);
  }

  printf("[INFO] Password: %d", password);
  
  fclose(in);
  return 0;
}
