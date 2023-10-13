#include "Banque.h"
#include <cstdlib> // srand, rand
#include <ctime>   // srand seed
#include <thread>  // Threads

using namespace std;

const int NB_THREAD = 10;   // N
const int NB_COMPTES = 100; // K comptes
const int SOLDEINITIAL = 500;

int main()
{
  vector<thread> threads;
  // TODO : creer des threads qui font ce qui est demandé
  srand(time(nullptr)); // Seed for the RNG

  threads.reserve(NB_THREAD);
  // Generate random account index
  int i = 1 + rand() / ((RAND_MAX + 1u) / NB_COMPTES);
  int j = 1 + rand() / ((RAND_MAX + 1u) / NB_COMPTES);
  // Random amount
  int m = 1 + rand() / ((RAND_MAX + 1u) / 100);

  for (auto &t : threads)
  {
    t.join();
  }

  // TODO : tester solde = NB_THREAD * JP
  return 0;
}
