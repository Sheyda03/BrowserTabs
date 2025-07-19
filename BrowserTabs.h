#pragma once
#include <string>
#include <chrono>

struct Tab
{
    std::string url;
    long timestamp;

    Tab(const std::string& url) : url(url) {
        timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }
};

class BrowserTabs {
    struct Node {
        Tab data;
        Node* next;
        Node* prev;

        Node(const Tab& data)
            : data(data), next(nullptr), prev(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    Node* current = nullptr;
    size_t count = 0;

    void copyFrom(const BrowserTabs& other);
    void moveFrom(BrowserTabs&& other);
    void free();

    void insertAfter(Node* position, const Tab& tabData);

    Node* mergeLists(Node* first, Node* second, bool (*compare)(const Tab&, const Tab&));
    Node* mergeSort(Node* list, bool (*compare)(const Tab&, const Tab&));
    Node* getMid(Node* list);

public:
    BrowserTabs();
    BrowserTabs(const BrowserTabs& other);
    BrowserTabs(BrowserTabs&& other) noexcept;

    BrowserTabs& operator=(const BrowserTabs& other);
    BrowserTabs& operator=(BrowserTabs&& other) noexcept;
    ~BrowserTabs();

    void go(const std::string& url);
    void insert(const std::string& url);
    void back();
    void forward();
    void remove();
    void print() const;
    bool isEmpty() const;
    void sort(const std::string& by);
};
bool compareByUrl(const Tab& a, const Tab& b);
bool compareByTime(const Tab& a, const Tab& b);