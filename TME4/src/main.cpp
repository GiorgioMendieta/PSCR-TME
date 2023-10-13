#include "Banque.h"
#include <cstdlib> // srand, rand
#include <ctime>   // srand seed
#include <thread>  // Threads
#include <iostream>

using namespace std;

const int NB_THREAD = 10;   // N
const int NB_COMPTES = 100; // K comptes
const int SOLDEINITIAL = 500;

void transaction(pr::Banque &b)
{
  for (int n = 0; n < 1000; n++)
  {
    // Generate random account index (1-NB_COMPTES)
    int i = (rand() % NB_COMPTES) + 1;
    int j = (rand() % NB_COMPTES) + 1;
    // Random amount (1-100)
    int m = (rand() % 100) + 1;

    b.transfert(i, j, m);

    // this_thread::sleep_for(chrono::milliseconds(m));
  }
}

int main()
{
  pr::Banque b(NB_COMPTES, SOLDEINITIAL); // Instantiate Bank object
  srand(time(nullptr));                   // Seed for the RNG

  vector<thread> threads;
  // TODO : creer des threads qui font ce qui est demandé
  threads.reserve(NB_THREAD); // Allocate space for the threads

  for (size_t i = 0; i < NB_THREAD; i++)
  {
    // Create threads and add them to the threads vector
    // threads.push_back(thread(transaction, ref(b)));
    threads.emplace_back(transaction, ref(b));
  }

  // Join all the threads
  for (auto &t : threads)
  {
    t.join();
  }

  // TODO : tester solde = NB_THREAD * JP
  return 0;
}
