#include "queue.h"


int main()
{
    chai_stl::Queue<int> queue({1, 2, 3, 4, 5});
    queue.print();

    
    queue.enqueue(1); queue.enqueue(2); queue.enqueue(3); queue.enqueue(4); queue.enqueue(5);
    queue.print();


    queue.dequeue();
    queue.print();


    queue.clear();
    queue.print();


    return 0;
}