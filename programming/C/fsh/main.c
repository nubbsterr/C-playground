// using bear to deal w/ clangd
// makefile intentionally gets rid of unused-param and set but unused param
// warnings cuz they're everywhere

// adapted from Stephen Brennan's awesome tutorial:
// https://brennan.io/2015/01/16/write-a-shell-in-c/#fnref:1
// named by a person/friend/acquiantance/human i met recently

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define fsh_BASE_LINE_BUFSIZE 1024
#define fsh_TOKEN_BUFSIZE 64
#define fsh_TOKEN_DELIMITERS " \t\r\n\a"

#define RED_COL "\033[0;31m"
#define GREEN_COL "\033[0;32m"
#define WHITE_COL "\033[0m"

// core
void fsh_loop(void);
char* fsh_readline(void);
char** fsh_splitargs(char*);
int fsh_exec_cmd(char**);
int fsh_launch(char**);

// build in commands
int fsh_cd(char** args);
int fsh_exit(char** args);
int fsh_help(char** args);
int num_builtins_func(void);

const char* builtins_str[] = {
    "cd",
    "help",
    "exit",
};

int (*builtins_func[])(char**) = {
    &fsh_cd,
    &fsh_help,
    &fsh_exit,
};

int num_builtins_func(void) { return sizeof(builtins_str) / sizeof(char*); }

int main(int argc, char** argv) {
  // loop the shell prompt
  fsh_loop();

  // shutdown
  return EXIT_SUCCESS;
}

void fsh_loop(void) {
  char* line;
  char** args;
  int status;

  do {
    printf("fsh $ ");
    line = fsh_readline();
    args = fsh_splitargs(line);
    status = fsh_launch(args);

    free(args);
    free(line);
  } while (status);
}

char* fsh_readline(void) {
  // getline() does exist and makes this a lot easier
  // but it is wut it is (legacy code is the future anyways)
  int bufsize = fsh_BASE_LINE_BUFSIZE, position = 0;

  char* buf = malloc(sizeof(char) * fsh_BASE_LINE_BUFSIZE);
  int c;
  if (buf == NULL) {
    fprintf(stderr, "fsh " RED_COL "[fatal]" WHITE_COL
                    ": failed to allocate input buffer!\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // get complete input per character, allocate more mem as needed
    // notice that 'c' is an int since we need to compare EOF to it
    // it will still behave perfectly fine as a char
    c = getchar();

    if (c == EOF || c == '\n') {
      buf[position] = '\0';  // terminate the command input
      return buf;
    } else {
      buf[position] = c;
    }
    position++;

    if (position >= bufsize) {
      // double our input capacity
      bufsize += fsh_BASE_LINE_BUFSIZE;
      buf = realloc(buf, bufsize);
      if (buf == NULL) {
        fprintf(stderr, "fsh " RED_COL "[fatal]" WHITE_COL
                        ": failed to reallocate input buffer!\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char** fsh_splitargs(char* cmd) {
  int bufsize = fsh_TOKEN_BUFSIZE, position = 0;
  // allocate space for each individual token string
  // we are storing a max of 64 args basically
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token;

  if (tokens == NULL) {
    fprintf(stderr, "fsh " RED_COL "[fatal]" WHITE_COL
                    ": failed to allocate token buffer!\n");
    exit(EXIT_FAILURE);
  }

  // strtok will return pointers to points in the string where
  // a token is found
  token = strtok(cmd, fsh_TOKEN_DELIMITERS);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += fsh_TOKEN_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));

      if (tokens == NULL) {
        fprintf(stderr, "fsh " RED_COL "[fatal]" WHITE_COL
                        ": failed to reallocate token buffer!\n");
        exit(EXIT_FAILURE);
      }
    }
    // passing NULL is akin to passing token as the string to parse
    // we will parse for the next token in this case
    token = strtok(NULL, fsh_TOKEN_DELIMITERS);
  }

  tokens[position] = NULL;
  return tokens;
}

int fsh_launch(char** args) {
  // no command given
  if (args[0] == NULL) {
    return 1;
  }

  // check builtins, executes builtin function if found
  for (int i = 0; i < num_builtins_func(); i++) {
    if (strcmp(args[0], builtins_str[i]) == 0) {
      return (*builtins_func[i])(args);
    }
  }

  // otherwise run cmd as normal
  return fsh_exec_cmd(args);
}

int fsh_exec_cmd(char** args) {
  // there are 2 ways to start processes on unix:
  //     1. load through Init, which is loaded by the kernel at boot,
  //         init runs during the lifetime of your session,
  //         and manages loading other processes for your system.
  //     2. use fork() to make a duplicate process under the parent process
  //         executing fork(). Returns 0 to the child proc, and the child PID
  //         to the parent proc. Then, use exec() to load an entirely new
  //         program; stopping the caller' program (child process). Note that a
  //         process called from exec() never returns unless an error occurs.
  // basically, just do method #2 lol. We can also use the wait() syscall to
  // obtains status info about the child process!

  pid_t childpid, ppid;
  int status;

  childpid = fork();
  if (childpid == 0) {
    // fork was good, now do exec
    if (execvp(args[0], args) == -1) {
      perror(RED_COL "fsh: " WHITE_COL);
    }
    // this exit will just exit from fsh_exec_cmd; the shell keeps going
    exit(EXIT_FAILURE);
  } else if (childpid < 0) {
    // error forking
    perror(RED_COL "fsh: " WHITE_COL);
  } else {
    // parent proc (us), get pid of child once it dies (command exits)
    do {
      ppid = waitpid(childpid, &status, WUNTRACED);
      // wait for the PID whilst it has yet to exit or killed through some
      // signal
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

// builtin functions
int fsh_cd(char** args) {
  if (args[1] == NULL) {
    fprintf(stderr, "fsh " RED_COL "[fatal]" WHITE_COL
                    ": expected argument for \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror(RED_COL "fsh: " WHITE_COL);
    }
  }
  return 1;
}

int fsh_help(char** args) {
  printf(GREEN_COL
         "nubbsterr's (nubbie) terrible shell (fsh, 'foreshell'). Based on "
         "Stephen "
         "Brennan's tutorial.\n" WHITE_COL);
  printf("The following are built in:\n");

  for (int i = 0; i < num_builtins_func(); i++) {
    printf("  %s\n", builtins_str[i]);
  }
  return 1;
}

int fsh_exit(char** args) { return 0; }
