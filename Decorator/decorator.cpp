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

void Decorator::operation()
{
	pc_->operation();
}
