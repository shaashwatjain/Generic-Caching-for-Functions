#include <iostream>
using namespace std;
#include "decorator.h"

Decorator::Decorator(Component* pc)
: pc_(pc)
{

}
Decorator::~Decorator()
{
}

int Decorator::operation(int arg)
{
	pc_->operation(arg);
}
