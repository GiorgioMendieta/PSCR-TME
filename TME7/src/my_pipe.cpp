#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h> // Wait

// File descriptor table (Father process)
// [0] Std in
// [1] Std out
// [2] Std err
// ---------- PIPE --------------
// [3] Pipe In
// [4] Pipe out

int main(int argc, char **argv)
{
  // Check for correct usage
  if (argc < 2)
  {
    perror("Usage: ./my_pipe argv1 \\| argv2");
    exit(1);
  }

  char **argv1;
  char **argv2;
  int tube_descriptor[2]; // Used to replace the R/W in the file descriptor table

  // Create new pipe
  if (pipe(tube_descriptor) == -1)
  {
    perror("Error creating pipe");
    exit(1);
  }

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
  argv[tube_index] = NULL;

  // Create the two sub argument vectors
  // There is no need to allocate new memory because we are using the original argv space in memory
  argv1 = argv + 1;
  argv2 = argv + tube_index + 1;

  // First child process (Write)
  // [0] Std in
  // [1] Std out  <- CLOSED, replaced by [4]
  // [2] Std err
  // ---------- PIPE --------------
  // [3] Ecriture <- CLOSED
  // [4] Lecture  <- Replaces [1] STDOUT

  // P1 | P2
  if (fork() == 0)
  {
    std::cout << "Writer: " << argv1[0] << std::endl;

    // File_d ancien, File_d nouveau
    close(tube_descriptor[0]);               // Close read end of the pipe
    dup2(tube_descriptor[1], STDOUT_FILENO); // STDOUT replaced by tube_descriptor[1]

    // Send the program name and the argument vector
    execv(argv1[0], argv1);
  }

  // Second child process (Read)
  // [0] Std in   <- CLOSED, replaced by [4]
  // [1] Std out
  // [2] Std err
  // ---------- PIPE --------------
  // [3] Ecriture <- Replaces [0] STDIN
  // [4] Lecture  <- CLOSED

  // P1 | P2
  if (fork() == 0)
  {
    std::cout << "Reader: " << argv2[0] << std::endl;
    // File_d ancien, File_d nouveau
    close(tube_descriptor[1]);              // Close write end of the pipe
    dup2(tube_descriptor[0], STDIN_FILENO); // stdin replaced by tube_descriptor[0]

    // Send the program name and the argument vector
    execv(argv2[0], argv2);
  }
  wait(0);
}