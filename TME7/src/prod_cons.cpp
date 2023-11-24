#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

using namespace std;
using namespace pr;

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

int main()
{
	Stack<char> *s = new Stack<char>();

	// Producer
	pid_t pp = fork();
	if (pp == 0)
	{
		producteur(s);
		return 0;
	}

	// Consumer
	pid_t pc = fork();
	if (pc == 0)
	{
		consomateur(s);
		return 0;
	}

	wait(0); // Producer
	wait(0); // Consumer

	delete s;
	return 0;
}
