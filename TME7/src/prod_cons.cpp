#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#include <sys/mman.h>
#include <sys/stat.h> /* Pour les constantes de mode */
#include <fcntl.h>

using namespace std;
using namespace pr;

bool ctrlc = false;

void producteur(Stack<char> *stack)
{
	char c;
	while (cin.get(c))
	{
		stack->push(c);
	}
}

void consomateur(Stack<char> *stack)
{
	while (true)
	{
		char c = stack->pop();
		cout << c << flush;
	}
}

Stack<char> *s = NULL;
int main()
{
	// Attendre la signal SIGINT pour s'arreter proprement
	signal(SIGINT, [](int sig)
		   { ctrlc = true; });

	int fd;
	void *sp; // Stack shared memory

	// Create named shared memory
	if ((fd = shm_open("/prod_cons", O_RDWR | O_CREAT, 0600)) == -1)
	{
		perror("shared memory error.");
		exit(1);
	}

	if (ftruncate(fd, sizeof(int)) == -1)
	{
		perror("ftruncate error.");
		exit(1);
	}

	if ((sp = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap error.");
		exit(1);
	}

	s = new (sp) Stack<char>();

	// Producer
	pid_t pp = fork();
	if (pp == 0)
	{
		signal(SIGINT, [](int sig)
			   { exit(1); });
		producteur(s);
		return 0;
	}

	// Consumer
	pid_t pc = fork();
	if (pc == 0)
	{
		signal(SIGINT, [](int sig)
			   { exit(1); });
		consomateur(s);
		return 0;
	}

	wait(0); // Producer
	wait(0); // Consumer

	while (!ctrlc)
	{
	}

	// Sortir proprement
	s->~Stack();
	munmap(sp, sizeof(int)); // delete s automatically
	shm_unlink("prod_cons");

	return 0;
}
