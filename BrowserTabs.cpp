#include <string>
#include <iostream>
#include <chrono>
#include "BrowserTabs.h"


bool BrowserTabs::isEmpty() const
{
    return count == 0;
}
BrowserTabs::BrowserTabs() : head(nullptr), tail(nullptr), current(nullptr), count(0)
{
    insert("about:blank");
    current = head;
}

BrowserTabs::~BrowserTabs()
{
    free();
}

void BrowserTabs::go(const std::string& url)
{
    if (isEmpty())
    {
        throw std::runtime_error("No tabs available.");
    }
    current->data.url = url;
    current->data.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void BrowserTabs::insert(const std::string& url)
{
    Tab newTab(url);

    if (isEmpty())
    {

        Node* newNode = new Node(newTab);
        head = tail = current = newNode;
    }
    else
    {
        insertAfter(current, newTab);
        current = current->next;
    }
    count++;
}

void BrowserTabs::insertAfter(Node* position, const Tab& tabData)
{
    Node* newNode = new Node(tabData);
    if (isEmpty())
    {
        head = tail = newNode;
    }
    else
    {
        newNode->next = position->next;
        newNode->prev = position;
        if (position->next)
        {
            position->next->prev = newNode;
        }
        position->next = newNode;
        if (position == tail)
        {
            tail = newNode;
        }
    }
    count++;
}

void BrowserTabs::back()
{
    if (current && current->prev)
    {
        current = current->prev;
    }
}

void BrowserTabs::forward()
{
    if (current && current->next)
    {
        current = current->next;
    }
}

void BrowserTabs::remove() {
    if (isEmpty())
    {
        throw std::runtime_error("No tabs to remove.");
    }
    Node* temp = current;
    if (head == tail)
    {
        head = tail = current = nullptr;
    }
    else if (current == head)
    {
        head = head->next;
        head->prev = nullptr;
        current = head;
    }
    else if (current == tail)
    {
        tail = tail->prev;
        tail->next = nullptr;
        current = tail;
    }
    else
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
        current = current->next;
    }

    delete temp;
    count--;

    if (isEmpty())
    {
        insert("about:blank");
        current = head;
    }
}

void BrowserTabs::print() const
{
    Node* iter = head;
    while (iter)
    {
        if (iter == current)
        {
            std::cout << "> ";
        }
        std::cout << iter->data.url << " (" << iter->data.timestamp << ")\n";
        iter = iter->next;
    }
    std::cout << std::endl;
}
BrowserTabs::BrowserTabs(const BrowserTabs& other)
{
    copyFrom(other);
}

BrowserTabs& BrowserTabs::operator=(const BrowserTabs& other) {
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}
BrowserTabs& BrowserTabs::operator=(BrowserTabs&& other) noexcept {
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}
void BrowserTabs::moveFrom(BrowserTabs&& other)
{
    head = other.head;
    tail = other.tail;
    current = other.current;
    count = other.count;

    other.head = other.tail = other.current = nullptr;
    other.count = 0;
}
BrowserTabs::BrowserTabs(BrowserTabs&& other) noexcept
{
    moveFrom(std::move(other));
}
void BrowserTabs::copyFrom(const BrowserTabs& other)
{
    Node* iter = other.head;
    while (iter)
    {
        insert(iter->data.url);
        iter = iter->next;
    }
    current = head;
}
void BrowserTabs::free()
{
    while (head)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = tail = current = nullptr;
    count = 0;
}

bool compareByUrl(const Tab& a, const Tab& b)
{
    if (a.url != b.url)
    {
        return a.url < b.url;
    }
    return a.timestamp < b.timestamp;
}

bool compareByTime(const Tab& a, const Tab& b)
{
    if (a.timestamp != b.timestamp)
    {
        return a.timestamp < b.timestamp;
    }
    return a.url < b.url;
}
BrowserTabs::Node* BrowserTabs::mergeLists(Node* first, Node* second, bool (*compare)(const Tab&, const Tab&))
{
    Node* resultBegin = nullptr, * resultEnd = nullptr;

    if (first && second) 
    {
        if (compare(first->data, second->data)) 
        {
            resultBegin = first;
            first = first->next;
        }
        else 
        {
            resultBegin = second;
            second = second->next;
        }
        resultEnd = resultBegin;
    }

    while (first && second)
    {
        if (compare(first->data, second->data)) 
        {
            resultEnd->next = first;
            first->prev = resultEnd;
            first = first->next;
        }
        else 
        {
            resultEnd->next = second;
            second->prev = resultEnd;
            second = second->next;
        }
        resultEnd = resultEnd->next;
    }

    if (first)
    {
        if (resultEnd) 
        {
            resultEnd->next = first;
            first->prev = resultEnd;
        }
        else 
        {
            resultBegin = first;
        }
    }
    if (second) 
    {

        if (resultEnd)
        {
            resultEnd->next = second;
            second->prev = resultEnd;
        }
        else 
        {
            resultBegin = second;
        }
    }

    return resultBegin;
}
BrowserTabs::Node* BrowserTabs::getMid(Node* list) {
    Node* slow = list;
    Node* fast = list->next;

    while (fast && fast->next) 
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
BrowserTabs::Node* BrowserTabs::mergeSort(Node* list, bool (*compare)(const Tab&, const Tab&)) {
    if (!list || !list->next)
    {
        return list;
    }      

    Node* mid = getMid(list);
    Node* left = list;
    Node* right = mid->next;
    mid->next = nullptr;

    left = mergeSort(left, compare);
    right = mergeSort(right, compare);

    return mergeLists(left, right, compare);
}
void BrowserTabs::sort(const std::string& by)
{
    if (isEmpty())
        return;

    if (by == "URL") 
    {
        head = mergeSort(head, compareByUrl);
    }
    else if (by == "TIME") 
    {
        head = mergeSort(head, compareByTime);
    }

    tail = head;
    while (tail->next)
    {
        tail = tail->next;
    }        
    current = head;
}
