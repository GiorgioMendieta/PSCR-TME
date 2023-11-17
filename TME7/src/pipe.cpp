#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  char **argv1;
  char **argv2;

  int tubeDesc[2];
  pid_t pid_fils;

  if (argc < 2)
  {
    perror("Usage: ./mon_program toto");
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

  std::cout << "Tube found at index : " << tube_index << std::endl;
}

// Std in
// Std out
// Std err