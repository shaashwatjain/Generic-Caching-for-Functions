#include <iostream>
using namespace std;
#include "concrete_decorator1.h"

Concrete_Decorator1::Concrete_Decorator1(Component *pc)
: Concrete_Decorator(pc)
{
}


int Concrete_Decorator1::operation(int arg) 
{
	cout << "decorated by message from concrete decorator one\n";
	Decorator::operation(arg);
    cout << "Test-2" << endl;
    return 0;
}
