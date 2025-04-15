#include <cstddef>
#include <iterator>
#include <new>
#pragma onece

#include "base_structure.h"
#include <initializer_list>
#include <iostream>


namespace chai_stl
{
template<typename T>
class Stack
{
public:
Stack(): top(nullptr), len(0) { }; // 默认构造函数
Stack(const T &val) {this->push(val);}; // 单参数构造函数
Stack(const std::initializer_list<T> &list) {for (auto &val : list) this->push(val); } // 初始化列表构造函数
void push(const T &val); // 入栈
void pop(); // 出栈
void clear(); // 清空栈
void print(); // 打印栈
bool isEmpty() { return this->top == nullptr; } // 判断栈是否为空
std::size_t size() { return this->len; } // 获取栈的大小
~Stack(); // 析构函数

Node<T>* top = nullptr; // 栈顶指针
std::size_t len = 0; // 栈长度
};

template<typename T>
void Stack<T>::push(const T &val)
{
    Node<T> *node = new Node<T>(val);
    node->next = this->top;
    this->top = node;
    this->len++;
}

template<typename T>
void Stack<T>::pop()
{
    if (this->top == nullptr) return;
    auto del_node = this->top;
    this->top = this->top->next;
    delete del_node;
    this->len--;
}

template<typename T>
void Stack<T>::clear()
{
    while (this->top != nullptr)
    {
        this->pop();
    }
    this->top = nullptr;
    this->len = 0;
}

template<typename T>
void Stack<T>::print()
{
    if (this->top == nullptr)
    {
        std::cout << "Stack is empty" << std::endl;
        return;
    }
    Node<T> *cur = this->top;
    while (cur != nullptr)
    {
        std::cout << cur->val << " -> ";
        cur = cur->next;
    }
    std::cout << "nullptr" << std::endl;
}

template<typename T>
Stack<T>::~Stack()
{
    this->clear();
    this->top = nullptr;
    this->len = 0;
}

}