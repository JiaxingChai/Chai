#include "stack.h"
#include <iostream>


int main()
{
    chai_stl::Stack<int> stack({1, 2, 3, 4, 5});
    stack.print();
    
    stack.push(1); stack.push(2); stack.push(3); stack.push(4); stack.push(5);
    stack.print();
    
    stack.pop();
    stack.print();
    
    stack.clear();
    stack.print();
    
    return 0;
}