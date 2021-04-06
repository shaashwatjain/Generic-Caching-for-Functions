#ifndef DECORATOR_H
#define DECORATOR_H
#include "component.h"
class Decorator : public Component
{
	public:
	Decorator(Component *pc);
	virtual void operation();
	virtual ~Decorator() = 0;
	private:
	Component *pc_;
};
#endif

