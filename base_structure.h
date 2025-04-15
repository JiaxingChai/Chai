#pragma once

namespace chai_stl {

template<typename T>
class Node
{
public:
    T val;
    Node *next = nullptr;
    Node(const T &v): val(v){};
};

}