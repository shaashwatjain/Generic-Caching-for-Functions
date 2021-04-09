#include <iostream>
using namespace std;
#include "concrete_decorator.h"

Concrete_Decorator::Concrete_Decorator(Component *pc)
: Decorator(pc)
{
}

int Concrete_Decorator::operation(int arg) 
{
	cout << "decorated by message from concrete decorator\n";
    // for (int i = 0; i < 10; ++i) {  // if cache hit
    //     if(cache[i][0] == arg)
    //     {
    //         cout << "Hit on " << arg << endl;
    //         ++victim_ind;
    //         victim_ind = victim_ind % 10;
    //         return cache[i][1];
    //     }
    // }

    // // if cache miss
    // cout << "Miss on " << arg << endl;
    
	// int val = Decorator::operation(arg);  // Original client's Undecorated function
    int val = 1;
    // this -> operation(arg);
    ++victim_ind;
    if (victim_ind <= 10){
        this -> operation(arg);  // This is the decorated function
        return arg;}    
    else
        return val;
    cout << "Test-1" << endl;
    // // updating the cache
    // cache[victim_ind][0] = arg;
    // cache[victim_ind][1] = val;

    // ++victim_ind;
    // victim_ind = victim_ind % 10;

   

    // return val;
}
