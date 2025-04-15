#pragma once
#include <initializer_list>
#include <iostream>
#include "base_structure.h" // 引入 Node<T> 的定义


namespace chai_stl
{


template<typename T>
class Queue{
public:
Queue(): front(nullptr), back(nullptr) { }; // 默认构造函数
Queue(const T &val); // 单参数构造函数
Queue(const std::initializer_list<T> &list) // 初始化列表构造函数
{
    for (auto &val : list)
        this->enqueue(val);
}

void enqueue(const T &val); // 入队
void dequeue(); // 出队
void clear(); // 清空队列
void print(); // 打印队列
bool isEmpty() { return this->front == nullptr; } // 判断队列是否为空
~Queue(); // 析构函数


Node<T> *front=nullptr; // 头指针
Node<T> *back=nullptr; // 尾指针
std::size_t len = 0; // 队列长度
};

template<typename T>
void Queue<T>::enqueue(const T&val)
{
    Node<T> *node = new Node<T>(val);
    if (this->front == nullptr) this->front = this->back = node;
    else
    {
        this->back->next = node;
        this->back = node;
    }
    this->len++;
}

template<typename T>
void Queue<T>::dequeue()
{
    if (this->front == nullptr) return;
    auto del_node = this->front;
    this->front = this->front->next;
    delete del_node;
    this->len--;
    if (this->front == nullptr) this->back = nullptr;
}

template<typename T>
Queue<T>::Queue(const T &val)
{
    this->enqueue(val);
}

template<typename T>
void Queue<T>::clear()
{
    while (this->front != nullptr)
    {
        auto del = this->front;
        this->front = this->front->next;
        delete del;
    }
    this->back = nullptr;
    this->len = 0;
}

template<typename T>
void Queue<T>::print()
{
    auto cur = this->front;
    while (cur != nullptr)
    {
        std::cout << cur->val << " -> ";
        cur = cur->next;
    }
    std::cout << "nullptr" << std::endl;
}

template<typename T>
Queue<T>::~Queue()
{
    this->clear();
}

}