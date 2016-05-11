#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
  int out[2];
  int in[2];
  int pid;
  posix_spawn_file_actions_t action;
  char* spawnedArgs[] = { "cat", "4300.txt", NULL };
  int status;
  char r[10240];
//  bzero(r, 1024);

  pipe(out);
  pipe(in);

  posix_spawn_file_actions_init(&action);
  posix_spawn_file_actions_adddup2(&action, out[0], 0);
  posix_spawn_file_actions_addclose(&action, out[1]);

  posix_spawn_file_actions_adddup2(&action, in[1], 1);
  posix_spawn_file_actions_addclose(&action, in[0]);
 
  posix_spawnp(&pid, spawnedArgs[0], &action, NULL, spawnedArgs, NULL);

  close(out[0]);
  close(in[1]);

//  write(out[1], "-al", "-al");
  close(out[1]);


  read(in[0], r, 10240);
  printf("Read data: \"%s\"\n", r);

  wait(&status);
  posix_spawn_file_actions_destroy(&action);

  return EXIT_SUCCESS;
}
