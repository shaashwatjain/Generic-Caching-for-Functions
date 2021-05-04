#pragma once

// #include "hash_utility.hpp"
#include <list>
#include<bits/stdc++.h>

struct Node
{
    int key, value, count;
    Node* prev;
    Node* next;
    Node(): count(0), prev(nullptr), next(nullptr) {}
    Node(int k, int v): key(k), value(v), count(1), prev(nullptr), next(nullptr) {}
};


// template<typename Cache_Size, typename R, typename ...Args>
class lfu_cache
{
    // Creating type alias for complex types
    // using Arguments = std::tuple<Args...>;
    // using Values = std::pair<R, typename std::list<Arguments>::iterator>;
    //
    // // Creating type alias for complex types
    // using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
    // using list_it_t = std::list<Arguments>::iterator;
    // using list_const_it_t = std::list<Arguments>::const_iterator;

  public:
    int size, capacity;
    std::unordered_map<int, Node*> count_map;
    std::unordered_map<int, Node*> map;

    Node* head;
    Node* tail;

    lfu_cache(int capacity)
    {
        head = new Node();
        head->count = 1; // for concise reasons, set this to 1
        count_map[1] = head; // for concise reasons

        tail = new Node();

        head->next = tail;
        tail->prev = head;
        // size related
        size = 0;
        assert(capacity >= 0);
        this->capacity = capacity;
    }

    void insert(Node* left, Node* cur)
    {
        //     left <-----------> next
        // ==> left <--> cur <--> next
        Node* n = left->next;
        left->next = cur;
        n->prev = cur;

        cur->prev = left;
        cur->next = n;
    }

    void remove(Node* cur)
    {
        //     prev <--> cur <--> next
        // ==> prev <-----------> next
        Node* p = cur->prev;
        Node* n = cur->next;
        p->next = n;
        n->prev = p;
    }

    void remove_and_adjust_count_map(Node* cur)
    {
        auto it = count_map.find(cur->count);
        assert(it != count_map.end()); // at least most_freq == cur
        Node* most_freq = it->second;

        if(most_freq == cur)
        {
            //     prev <--> cur
            // ==> ^^^^ <--------- count_map[cur->count] = prev
            if(cur->prev->count == cur->count) it->second = cur->prev;
            else count_map.erase(cur->count); // there is no node with `count`
        }
    }

    void remove_least_frequently_used()
    {
        Node* cur = head->next;
        assert(cur != tail);
        remove(cur); // remove it from the linked list
        map.erase(cur->key);
        remove_and_adjust_count_map(cur);
    }

    void increment_and_reorder(Node* cur)
    {
        auto count_it = count_map.find(cur->count); // the original count
        auto count_it2 = count_map.find(cur->count + 1); // the incremented count
        assert(count_it != count_map.end());

        // Case 1: If the cur->count+1 exists
        // Case 2: If the cur->count does not exist
        if(count_it2 != count_map.end())
        {
            // pass to the previous one
            if(count_it->second == cur)
            {
                if(cur->prev->count == cur->count) count_it->second = cur->prev;
                else count_map.erase(cur->count);
            }

            // we put this node on the right of it
            Node* left = count_it2->second;
            remove(cur);
            insert(left, cur);
            count_it2->second = cur; // the new most recently used
        }
        else
        {
            // record the new most recently used for cur->count+1
            count_map[cur->count + 1] = cur;

            if(count_it->second == cur)
            {
                // if this node is the most recently used among this count
                if(cur->prev->count == cur->count) count_it->second = cur->prev;
                else count_map.erase(cur->count);
            }
            else
            {
                Node* left = count_it->second;
                remove(cur);
                insert(left, cur);
            }
        }

        // increment its count
        cur->count++;
    }

    int get(int key)
    {
        auto it = map.find(key);

        if(it != map.end())
        {
            Node* cur = it->second;
            increment_and_reorder(cur);
            return cur->value;
        }

        return -1;
    }

    void put(int key, int value)
    {
        if(capacity == 0) return;

        auto it = map.find(key);

        if(it != map.end())
        {
            Node* cur = it->second;
            cur->value = value; // update the value
            increment_and_reorder(cur);
        }
        else
        {
            // remove the least frequently used node
            if(size == capacity) remove_least_frequently_used();
            else size++;

            // instantiate a new node
            Node* cur = new Node(key, value);
            map[key] = cur;
            // insert (key, value) node on the right of count_map[1] (initially is `head`)
            auto count_it = count_map.find(1);
            assert(count_it != count_map.end());
            Node* left = count_it->second;
            // left <--> cur
            //           ^^^ <--- become the most recently used for nodes with count 1
            insert(left, cur);
            count_it->second = cur;
        }
    }
};


// template<typename Cache_Size, typename R, typename ...Args>
// void lfu_cache<Cache_Size, R, Args...>::flush_cache()
// {
//     cache_.clear();  // Clear the cache
//
//     // Clear the bookkeeping
//     if constexpr(Cache_Size::type)  usage_tracker_.clear();
// }

int main()
{
    lfu_cache cache(2);	// cache capacity 2
    cache.put(5, 2);
    std::cout << cache.get(5) << std::endl;
    std::cout << cache.get(6) << std::endl;
    cache.put(6, 1);
    cache.put(6, 5);
    std::cout << cache.get(6) << std::endl;
    std::cout << cache.get(5) << std::endl;
    cache.put(8, 8);
    std::cout << cache.get(6) << std::endl;
    std::cout << cache.get(8) << std::endl;
}

