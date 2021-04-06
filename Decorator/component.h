#ifndef COMPONENT_H
#define COMPONENT_H
// pure virtual dtor should also have a definition
class Component
{
	public:
	virtual ~Component() = 0;
	virtual void operation() = 0; // pure virtual
};
#endif
