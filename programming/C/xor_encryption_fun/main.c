#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // no key given
  if (argc < 2) {
    fprintf(stderr, "No key given! Format: ./main <int: key>\n");
    return 1;
  }

  char supersecretmsg[] = "super secret message that you can't see";
  time_t unixepoch = time(NULL);
 
  for (long unsigned int i = 0; i < strlen(supersecretmsg); i++) {
    supersecretmsg[i]^=unixepoch; // encrypt each char in string 
  }

  printf("Encrypted message: %s\n", supersecretmsg);

  for (long unsigned int i = 0; i < strlen(supersecretmsg); i++) {
    supersecretmsg[i]^=(atoi(argv[1])); // attempt decryption with given key 
  }
  printf("Attempted decryption w/ given key: %s\n", supersecretmsg);
  return 0;
}
