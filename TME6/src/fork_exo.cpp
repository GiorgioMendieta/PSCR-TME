#include <iostream>
#include <unistd.h>   // System calls
#include <sys/wait.h> // Wait
#include <vector>     // Vector
#include <string>

void save_process(std::vector<std::pair<std::string, int>> &processes, std::string name, int pid)
{
  // std::cout << "Saving process PID=" << pid << " with name " << name << std::endl;

  std::pair<std::string, int> p; // Key (string), Value (PID) pair
  p.first = name;
  p.second = pid;
  processes.push_back(p);
}

std::string get_process_name(std::vector<std::pair<std::string, int>> &processes, int pid)
{
  // printf("Looking for PID=%d name\n", pid);
  // For each pair of word & occurrence in word_occurrence vector
  for (auto &p : processes)
  {
    // If word is found, increment the occurrence count
    if (p.second == pid)
    {
      // std::cout << "Process found! Name: " << p.first << std::endl;
      return p.first;
    }
  }
  return nullptr;
}

int main()
{
  const int N = 3;
  int nbFils = 0;
  std::vector<std::pair<std::string, int>> processes;

  printf("\n****** Father process 👨🏻 ******\n");
  std::cout << "Main PID = " << getpid() << std::endl;
  std::cout << "Nombre de processus à creer N = " << N << std::endl;

  for (int i = 1, j = N; i <= N && j == N && fork() == 0; i++)
  {
    printf("\n  **** Child %d created 👶🏻 ****\n", i);
    save_process(processes, std::string("Child " + std::to_string(i)), getpid());

    // if (getpid() != getppid())
    // {
    //   nbFils++;
    // }

    std::cout << "  PPID = " << getppid() << std::endl;
    std::cout << "  PID = " << getpid() << std::endl;

    std::cout << "   i:j - " << i << ":" << j << std::endl;

    for (int k = 1; k <= i && j == N; k++)
    {
      if (fork() == 0)
      {
        printf("\n    ** Grandchild %d created 👶🏻👶🏻 **\n", k);
        save_process(processes, std::string("Grandchild " + std::to_string(k)), getpid());

        if (getpid() != getppid())
        {
          nbFils++;
        }

        std::cout << "    PPID = " << getppid() << std::endl;
        std::cout << "    PID = " << getpid() << std::endl;

        j = 0;
        std::cout << "     k:j - " << k << ":" << j << std::endl;
      }
    }
  }

  for (int i = 0; i < nbFils; i++)
  {
    std::string pname = get_process_name(processes, getpid());
    wait(NULL);
    std::cout << pname << " terminated 💀\n";
  }

  return 0;
}
