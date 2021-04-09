#include <iostream>
using namespace std;
#include "concrete_component.h"


// Original client code
int Concrete_Component::operation(int arg)
{
	cout << "concrete component : operation\n";
    this -> operation();  // what would this be if the object calling operation is of derived class
    return arg * 10;
}
