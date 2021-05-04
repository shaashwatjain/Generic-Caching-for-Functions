# Generic Programming Project: Generic caching for Functions 
## Project ID: 16
### Team Members:
| Name | SRN |
|:-:|:-:|
| Rishit C | PES1201800316 |
| Shaashwat J | PES1201802346 |
| Raghav A | PES1201800312 |


## Introduction:

In this project our aim is to create a generic wrapper for a user defined function to give it a cache with a user-selected policy for replacement, i.e LRU, MFU, MRU, LFU, Random etc.
The wrapper returns a function object which can then be used by the user to make multiple calls to their function and when repeated calls are made, the system uses its cache rather than recalculating the value again.

## How to Execute the Project:

```bash
$ cd Cache
$ make
$ ./a.out
```
**Note:** In the makefile one can enable or disable the diagnostic print statements if needed. Note that using the DEBUG mode may write a lot of messages to stdout.

## Compiler Flags and Significance:

`-DDEBUG=${DEBUG_VAL}`  :  
Allows us to enable (non-zero value for `DEBUG_VAL`) or disable (zero value for `DEBUG_VAL`)  debug mode wherein the code prints more data about the cache. This mode is useful during debugging.

`-Wall`  :  
Enables recommended compiler warnings

`-Werror`  :  
Make all warnings into errors.

`-std=c++2a`  :  
Use the C++20 standard

`-pedantic`  :  
 Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++.

`-Weffc++`  :  
 Warn about violations of the following style guidelines from Scott Meyers' Effective C++ series of books:
*   Define a copy constructor and an assignment operator for
    classes with dynamically-allocated memory.
*   Prefer initialization to assignment in constructors.
*   Have "operator=" return a reference to *this.
*   Don't try to return a reference when you must return an
    object.
*   Distinguish between prefix and postfix forms of increment
    and decrement operators.
*   Never overload "&&", "||", or ",".

`-fstack-protector-all`  :   
Stack smashing protector

`-fstack-clash-protection`  :  
Increased reliability of stack overflow detection

`-g`  :  
Generate debugging information


## Novelty:

Our project makes extensive use of Modern C++ programming techniques. Our code uses C++20 features such as concepts and better compile time types deduction. We have also made use of extensive compile time programming and checks to ensure that we are able to achieve a zero-abstraction overhead for the user and they only pay for what they use, which in our case is extra space for the cache and extra computation for the cache lookup and eviction policy.  
The user can also use pointer types with our cache provided they use a `shared_ptr<T>` so that the pointer does not dangle until the the elements are removed from the cache or the cache goes out of scope and the user does not hold the `shared_ptr<T>`. This ensures no cache leaks on part of the user and cache, while taking advantage of the caching infrastructure.


## Caching Overheads:

Memory for the unordered_map which is used to implement the cache
Memory for the book-keeping for implementing the cache replacement policy
The
CPU overhead for calculating the hash of the key every lookup
CPU overhead for maintaining the the book-keeping for implementing the cache replacement policy

## Caching Policies Implemented:

1. **Least Recently Used (LRU)**  
Description: In this policy, when the cache becomes full, the least recently used key-value pair in the map is selected and evicted from the cache to make space for the incoming element.
    * **Time Complexity:** O(1) for insertion and deletion of key-value pairs.
    * **Space Complexity:** O(n) where n is the number of unique elements given as argument from the cached function.

2. **Most Recently Used (MRU)**  
Description: In this policy, when the cache becomes full, the least recently used key-value pair in the map is selected and evicted from the cache to make space for the incoming element.
    * **Time Complexity:** O(1) for insertion and deletion of key-value pairs.
    * **Space Complexity:** O(n) where n is the number of unique elements given as argument from the cached function.

3. **Least Frequently Used (LFU)**  
Description: In this policy, when the cache becomes full, the least frequently used key-value pair in the map is selected and evicted from the cache to make space for the incoming element.
    * **Time Complexity:** O(1) for insertion and deletion of key-value pairs.
    * **Space Complexity:** O(n) where n is the number of unique elements given as argument from the cached function.

4. **Most Frequently Used (MFU)**  
Description: In this policy, when the cache becomes full, the most frequently used key-value pair in the map is selected and evicted from the cache to make space for the incoming element.
    * **Time Complexity:** O(1) for insertion and deletion of key-value pairs.
    * **Space Complexity:** O(n) where n is the number of unique elements given as argument from the cached function.

5. **Random Eviction (RE)**  
In this policy, when the cache becomes full, the key-value pair in the map is selected at random and evicted from the cache to make space for the incoming element.
    * **Time Complexity:** O(n) for insertion and deletion of key-value pairs.
    * **Space Complexity:** O(n) where n is the number of unique elements given as argument from the cached function.

## Discussion about Implementation:

In the client's file, the client needs to only include our header file called: cache.hpp
After that the user must first wrap their function in an std::function object and then pass it as a parameter to the constructor of the object of type my_cache. The user must also specify additional parameters about the:  
* **Caching policy**
  - **Requirement:** Mandatory
  - **Object of Type:** 
    - ```policy::LRU_CACHE```
    - ```policy::MRU_CACHE```
    - ```policy::LFU_CACHE```
    - ```policy::MFU_CACHE```
    - ```policy::RANDOM_CACHE```
* **Size of the cache**
    - **Requirement:** Optional
    - **Object of Type:**
      - ```cache_size::UNLIMITED```
      - ```cache_size::RESTRICTED<Size>```
        - ```Size``` is a value of type ```std::size_t```, that signifies the size of the cache the user desires
    - **Default Value:** ```cache_size::UNLIMITED```

## Conclusion:

In this project we have created a generic cache wrapper for user defined functions. The wrapper also provides the user with the choice of selecting the size of and eviction policy of the cache. We also demonstrated that the cached wrapped version of the user defined function was faster than the non cache wrapped version of the same function across various types of test cases, cache sizes and cache eviction policies. The implementation uses modern C++ (here C++20) features and takes advantage of compile time programming in order to make sure that the generated code is exactly what the user required.
