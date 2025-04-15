#pragma once
#include <iostream>
#include <initializer_list>
#include "base_structure.h" // 引入 Node<T> 的定义

namespace chai_stl{
template<typename T>
class Linked_list
{
public:
    Linked_list(): head(nullptr), tail(nullptr) { }; // 默认构造函数
    Linked_list(const T &val); // 单参数构造函数
    Linked_list(const std::initializer_list<T> &list); // 初始化列表构造函数

    std::size_t get_len() {return this->len;};
    void push_back(const T &val); // 尾插法
    void push_front(const T &val); // 头插法
    void pop_back(); // 尾删法
    void pop_front(); // 头删法
    void insert(const std::size_t index, const T& val); // 插入
    void erase(const std::size_t index); // 删除
    void clear(); // 清空链表
    void print(); // 打印链表
    void reverse(); // 反转链表
    void sort(bool (*cmp)(const T&, const T&)); // 自定义排序链表
    void merge(Linked_list<T>& other); // 合并链表
    bool isEmpty() { return this->head == nullptr; } // 判断链表是否为空
    ~Linked_list(); // 析构函数

    Node<T> *head=nullptr; // 头指针
    Node<T> *tail=nullptr; // 尾指针
    std::size_t len = 0; // 链表长度
};

template<typename T>
Linked_list<T>::Linked_list(const T &val)
{
    Node<T>* node = new Node<T>(val);
    this->head = this->tail = node;
    this->len++;
}

template<typename T>
void Linked_list<T>::push_back(const T& val)
{
    Node<T> *node = new Node<T>(val);
    if(this->head == nullptr) this->head = this->tail = node;
    else
    {
        this->tail->next = node;
        this->tail = node;
    }
    this->len++;
}


template<typename T>
Linked_list<T>::Linked_list(const std::initializer_list<T> &list)
{
    for (const auto& val : list) {
        this->push_back(val);
    }
}

template<typename T>
Linked_list<T>::~Linked_list()
{
    this->clear();
}

template<typename T>
void Linked_list<T>::push_front(const T &val)
{
    Node<T> *node = new Node<T>(val);
    node->next = this->head;
    this->head = node;
    if (this->tail == nullptr) this->tail = node;
    this->len++;
}

template<typename T>
void Linked_list<T>::print()
{
    auto cur = this->head;
    while (cur != nullptr)
    {
        std::cout << cur->val << " -> ";
        cur = cur->next;
    }
    std::cout << "nullptr" << std::endl;
}

template<typename T>
void Linked_list<T>::insert(const std::size_t index, const T& val)
{
    auto cur = this->head;
    if (index <= 0)
    {
        this->push_front(val);
    }
    else if (index >= this->len)
    {
        this->push_back(val);
    }
    else
    {
        for (std::size_t i = 0; i < index - 1; i++)
        {
            cur = cur->next;
        }
        Node<T> *node = new Node<T>(val);
        node->next = cur->next;
        cur->next = node;
        this->len++;
    }
}

template<typename T>
void Linked_list<T>::erase(const std::size_t index)
{
    if (index <= 0 && this->head != nullptr)
    {
        auto cur = this->head;
        this->head = this->head->next;
        delete cur;
    }
    else if (index >= this->len - 1 && this->tail != nullptr)
    {
        auto cur = this->head;
        while (cur->next != this->tail) cur = cur->next;
        delete this->tail;
        this->tail = cur;
        this->tail->next = nullptr;
    }
    else
    {
        auto cur = this->head;
        for (std::size_t i = 0; i < index - 1; i++) cur = cur->next;
        auto del_node = cur->next;
        cur->next = del_node->next;
        delete del_node;
    }
    this->len--;
}

template<typename T>
void Linked_list<T>::clear()
{
    auto cur = this->head;
    while (cur != nullptr)
    {
        auto next = cur->next;
        delete cur;
        cur = next;
    }
    this->head = this->tail = nullptr;
    this->len = 0;
}

template<typename T>
void Linked_list<T>::reverse()
{
    if (this->head == nullptr) return;
    auto prev = this->head;
    auto cur = this->head->next;
    while (cur != nullptr)
    {
        auto next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    this->tail = this->head;
    this->head = prev;
    this->tail->next = nullptr;
}

template<typename T>
void Linked_list<T>::sort(bool (*cmp)(const T&, const T&))
{
    if (this->head == nullptr) return;
    auto cur = this->head;
    while (cur != nullptr)
    {
        auto next = cur->next;
        while (next != nullptr)
        {
            if (cmp(cur->val, next->val))
            {
                std::swap(cur->val, next->val);
            }
            next = next->next;
        }
        cur = cur->next;
    }
}

template<typename T>
void Linked_list<T>::merge(Linked_list<T>& other)
{
    if (other.head == nullptr) return;
    if (this->head == nullptr)
    {
        this->head = other.head;
        this->tail = other.tail;
        this->len = other.len;
        return;
    }
    this->tail->next = other.head;
    this->tail = other.tail;
    this->len += other.len;

    other.head = nullptr;
    other.tail = nullptr;
    other.len = 0;
}

} // chai_stl;