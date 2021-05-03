#include<iostream>
#include<list>
#include<unordered_map>
#include <chrono>
using namespace std;

auto start = std::chrono::high_resolution_clock::now();

// template<typename T>
struct Node
{
    int key;
    Node* next;
    Node* prev;

    Node(int k) : key(k), next(nullptr), prev(nullptr)
    {}
};

class LinkedList
{
    Node* head;
    Node* prev;
    int max_size;
    int c_size = 0;

  public:
    LinkedList(int msize)
    {
        max_size = msize;
        head = nullptr;
        prev = nullptr;
    }

    Node* insert(int key)
    {
        Node* node = new Node(key);

        if (head == nullptr && prev == nullptr)
        {
            head = node;
            prev = node;
            c_size++;
            return node;
        }

        prev->next = node;
        node->prev = prev;
        c_size++;
        prev = node;
        return node;
    }

    int removeHead()
    {

        int data = head->key;
        Node* newHead = head->next;
        head = nullptr;
        delete head;
        head = newHead;
        c_size--;
        return data;
    }

    void MRU_Node(Node* node)
    {

        if (node == prev || c_size <= 1)
            return;

        if (node == head)
        {
            Node* newHead = head->next;
            newHead->prev = nullptr;
            head = newHead;
            //
        }
        else
        {
            Node* prevNode = node->prev;
            Node* nextNode = node->next;

            //remove from this list
            prevNode->next = nextNode;
            nextNode->prev = prevNode;
        }

        node->prev = prev;
        prev->next = node;
        prev = node;
        prev->next = nullptr;

    }

    void printNode()
    {
        Node* pNode = head;
        cout << "HEAD->";

        while (pNode != nullptr)
        {
            cout << pNode->key << "(" << pNode << ")" << "->";
            pNode = pNode->next;
        }

        cout << "prev\n";
    }

    int size()
    {
        return c_size;
    }

    int capacity()
    {
        return max_size;
    }
};

class LRUCache
{
    int listSize_;
    LinkedList* cacheKeys_;
    unordered_map<int, Node*> keyAddrMap_;
    unordered_map<int, int> keyValueMap_;

  public:
    LRUCache(int capacity)
    {
        listSize_ = capacity;
        cacheKeys_ = new LinkedList(listSize_);
    }

    int get(int key)
    {
        auto iter_key = keyValueMap_.find(key);
        int val = (iter_key == keyValueMap_.end()) ? -1 : iter_key->second;

        if (val == -1)
            return val;

        auto iter_keyAddr = keyAddrMap_.find(key);
        Node* addrNode = iter_keyAddr->second;

        cacheKeys_->MRU_Node(addrNode);
        return val;
    }

    void put(int key, int value)
    {
        auto iter_key = keyAddrMap_.find(key);

        if (iter_key != keyAddrMap_.end())
        {
            keyValueMap_[key] = value;
            Node* addrNode = iter_key->second;
            cacheKeys_->MRU_Node(addrNode);
            return;
        }

        /* if cache size is not full */
        if (cacheKeys_->size() < listSize_)
        {

            /* insert key in cache */
            Node* addr = cacheKeys_->insert(key);

            /* update addr map */
            keyAddrMap_[key] = addr;

            /* update value map */
            keyValueMap_[key] = value;

        }
        else
        {
            /* when cache is full */
            int removedKey = cacheKeys_->removeHead();

            /* remove Key from value and addrMap */
            keyAddrMap_.erase(removedKey);
            keyValueMap_.erase(removedKey);

            Node* addr = cacheKeys_->insert(key);
            /* update addr map */
            keyAddrMap_[key] = addr;

            /* update value map */
            keyValueMap_[key] = value;
        }

    }

};

int main()
{
    LRUCache cache(2);	// cache capacity 2
    cache.put(5, 2); // (key value pair)
    std::cout << cache.get(5) << std::endl;
    std::cout << cache.get(6) << std::endl;
    cache.put(6, 1);
    cache.put(6, 5);
    std::cout << cache.get(6) << std::endl;
    std::cout << cache.get(5) << std::endl;
    cache.put(8, 8);
    std::cout << cache.get(6) << std::endl;
    std::cout << cache.get(8) << std::endl;

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}
