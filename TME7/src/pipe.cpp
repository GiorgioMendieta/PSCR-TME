#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// File descriptor table (Father process)
// [0] Std in
// [1] Std out
// [2] Std err
//---------------
// [3] Ecriture (replaces [0]) <- Pipe
// [4] Lecture  (replaces [1]) <- Pipe

int main(int argc, char **argv)
{
  // Check for correct usage
  if (argc < 2)
  {
    perror("Usage: ./mon_program toto");
    exit(1);
  }

  char **argv1;
  char **argv2;
  int fd[2]; // Used to replace the R/W in the file descriptor table
  pipe(fd);  // Create new pipe

  // Search until we find the pipe
  int tube_index = 0;
  while (argv[++tube_index][0] != '|')
  {
    // Reached the end of the argument vector without finding a tube
    if (tube_index == (argc - 1))
    {
      perror("No pipe found.");
      exit(1);
    }
  }

  // Replace the pipe by a NULL character
  argv[tube_index][0] = '\0';

  // Create the two sub argument vectors
  argv1 = argv + 1;
  argv2 = argv + tube_index;

  // First child process (Write)
  if (fork() == 0)
  {
    // File_d ancien, File_d nouveau
    dup2(1, fd[0]);
    close(fd[1]); // Write end of the pipe
    // Send the program name and the argument vector
    execv(argv1[0], argv1);
  }

  // Second child process (Read)
  if (fork() == 0)
  {
    // File_d ancien, File_d nouveau
    close(fd[0]); // Read end of the pipe
    dup2(0, fd[1]);
    // Send the program name and the argument vector
    execv(argv2[0], argv2);
  }
}