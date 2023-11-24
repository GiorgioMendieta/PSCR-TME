#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>

namespace pr
{

#define STACKSIZE 100

	template <typename T>
	class Stack
	{
		T tab[STACKSIZE];
		size_t sz;

		sem_t mutex;
		sem_t s_prod; // Producer
		sem_t s_cons; // Consumer

	public:
		Stack() : sz(0)
		{
			memset(tab, 0, sizeof tab);

			sem_init(&mutex, 1, 1);
			sem_init(&s_prod, 1, 0);
			sem_init(&s_cons, 1, sz);
		}

		~Stack()
		{
			sem_destroy(&mutex);
			sem_destroy(&s_prod);
			sem_destroy(&s_cons);
		}

		T pop()
		{
			sem_wait(&s_prod);
			sem_wait(&mutex);
			// bloquer si vide
			T toret = tab[--sz];
			return toret;
			sem_post(&mutex);
			sem_post(&s_cons);
		}

		void push(T elt)
		{
			// bloquer si plein
			tab[sz++] = elt;
		}
	};

}
