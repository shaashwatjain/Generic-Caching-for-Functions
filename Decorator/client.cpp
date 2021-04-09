#include <iostream>
using namespace std;
#include "component.h"
#include "concrete_component.h"
#include "concrete_decorator.h"
#include "concrete_decorator1.h"

int main()
{
	Component *pc;
	//pc = new Component; // compile time error; instantiating an abstract class
	//delete pc;
	
	pc = new Concrete_Component();
	cout << pc->operation(1) << endl;  // Without caching
	cout << "--------------------------------\n";
	
	//new Decorator(); // NO; abstract aclass
	pc = new Concrete_Decorator(pc);
	// With caching
    pc->operation(1); 
    pc->operation(2); 
    pc->operation(2); 
    pc->operation(3); 
	pc->operation(1); 
	cout << "--------------------------------\n";
	// pc = new Concrete_Decorator1(pc);
	// pc->operation();
	
}




