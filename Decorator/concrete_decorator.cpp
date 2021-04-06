#include <iostream>
using namespace std;
#include "concrete_decorator.h"

Concrete_Decorator::Concrete_Decorator(Component *pc)
: Decorator(pc)
{
}


void Concrete_Decorator::operation() 
{
	cout << "decorated by message from concrete decorator\n";
	Decorator::operation();
}
