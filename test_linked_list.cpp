#include "linked_list.h"


int main()
{
    chai_stl::Linked_list<int> a = 999;
    a.push_front(1); a.push_front(2); a.push_front(3); a.push_front(4);
    a.print();

    a.insert(1000, -100);
    a.print();

    a.erase(2);
    a.print();

    // a.reverse();
    // a.print();

    a.sort([](const int &a, const int &b) { return a > b; });
    a.print();

    chai_stl::Linked_list<int> b({1, 2, 3, 4, 5});
    a.merge(b);
    a.print();

    return 0;
}