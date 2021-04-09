#ifndef CONCRETE_DECORATOR_H
#define CONCRETE_DECORATOR_H
#include "decorator.h"
class Concrete_Decorator : public Decorator
{
    int cache[10][2] = {};
    int victim_ind = 0;

	public:
	Concrete_Decorator(Component *pc);
	virtual int operation(int) override;
};
#endif
