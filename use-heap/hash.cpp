/*
ECE-365 Project 2: useHeap
Layth Yassin
Professor Sable

This file includes implementations of functions from hashTable class. Linear probing is used to resolve collisions
and the polynomial rolling hash function is used as the hash function.
*/

#include "hash.h"

// length of primeNumbers array
#define primeNumbersLength sizeof(primeNumbers)/sizeof(primeNumbers[0])

// good precomputed prime numbers to use for resizing the hash table; numbers from https://planetmath.org/goodhashtableprimes
int primeNumbers[] = {98317, 393241, 1572869, 6291469, 25165843, 100663319, 402653189, 1610612741};

// constructor to initialize hash table
hashTable::hashTable(int size) {
    // computes smallest good prime number greater than given size to serve as initial capacity of hash table
    capacity = getPrime(size);
    data.resize(capacity);
    filled = 0;
    // initialize all elements of data vector to appropriate values
    for (int i = 0; i < capacity; i++) {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
        data[i].pv = nullptr;
    }
}

// inserts given key into hash table, uses linear probing to resolve collisions
int hashTable::insert(const std::string &key, void *pv) {
    double loadFactor = (double) filled/capacity;
    // if load factor exceeds 0.5, perform rehash
    if (loadFactor >= 0.5) {
        // if rehash fails, return 2
        if (!rehash()) {
            return 2;
        }
    }
    // returns 1 if the key is already in the hash table 
    if (contains(key)) {
        return 1;
    }

    int hashIndex = hash(key);
    int probe = hashIndex;

    // iterate through data vector in linear manner (linear probing) until program reaches unoccupied data element
    while (data[probe].isOccupied) {
        if (data[probe].isDeleted) {
            break;
        }
        else {
            // if probe reaches end of data vector, set probe to 0 to allow for wraparound
            if (probe == capacity - 1) {
                probe = 0;
            }
            // otherwise increment probe by one to search linearly
            else {
                probe++;
            }
        }
    }
    
    // by the first 2 conditional statements, the program is guaranteed to find an unoccupied slot in data vector;
    // insertion occurs at this guaranteed slot
    data[probe].key = key;
    data[probe].isOccupied = true;
    data[probe].isDeleted = false;
    data[probe].pv = pv;
    filled++;
    return 0;
}

// checks if hash table contains given key
bool hashTable::contains(const std::string &key) {
    if (findPos(key) == -1) {
        return false;
    }
    else {
        return true;
    }
}

// polynomial rolling hash function, code based on https://cp-algorithms.com/string/string-hashing.html
int hashTable::hash(const std::string &key) {
    const int p = 31;
    const int m = 1e9 + 9;
    unsigned hash_value = 0;
    long long p_pow = 1;
    for (char c : key) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return (hash_value % capacity);
}

// returns position of item if found, otherwise returns -1
int hashTable::findPos(const std::string &key) {
    int hashIndex = hash(key);

    // mimics path taken by linear probing search for empty position to insert; if data[hashIndex]
    // is empty, then given key is not in hash table
    while (data[hashIndex].isOccupied) {
        if (data[hashIndex].key == key) {
            // checks if matched key is deleted
            if (data[hashIndex].isDeleted) {
                return -1;
            }
            else {
                return hashIndex;
            }
        }
        else {
            // if index reaches end of data vector, set index to 0 to allow for wraparound
            if (hashIndex == capacity) {
                hashIndex = 0;
            }
            // otherwise increment index by one to search linearly
            else {
                hashIndex++;
            }
        }
    }
    return -1;
}

// makes hash table bigger, returns true on success and false otherwise
bool hashTable::rehash() {
    // copy data to another vector
    std::vector<hashItem> oldVect = data;
    // compute next smallest good prime number greater than 2 times the current capacity
    // to serve as new capacity of hash table after rehashing
    int newCapacity = getPrime(2 * capacity);
    // attempts to allocate new memory for rehashed table, returns false if allocation fails
    try {
        data.resize(newCapacity);
    }
    catch (std::bad_alloc) {
        return false;
    }
    capacity = newCapacity;
    filled = 0;

    // initialize all elements of rehashed data vector to appropriate values
    for (int i = 0; i < newCapacity; i++) {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
        data[i].pv = nullptr;
    }

    // insert old keys into rehashed data vector
    for (int i = 0; i < oldVect.size(); i++) {
        if ((oldVect[i].isOccupied) && (!oldVect[i].isDeleted)) {
            insert(oldVect[i].key, oldVect[i].pv);
        }
    }
    return true;
}

// returns smallest good prime number greater than given size to serve as the size of hash table
unsigned int hashTable::getPrime(int size) {
    for (int i = 0; i < primeNumbersLength; i++) {
        if (size <= primeNumbers[i]) {
            return primeNumbers[i];
        }
    }
    return primeNumbers[primeNumbersLength - 1];
}

// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return nullptr.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
// returns pointer associated with specified key
void *hashTable::getPointer(const std::string &key, bool *b) {
    int pos = findPos(key);

    if (pos != -1) {
        if (b != nullptr) {
            *b = true;
        }
        return data[pos].pv;
    }
    else {
        if (b != nullptr) {
            *b = false;
        }
        return nullptr;
    }
}

// sets the pointer associated with the specified key
int hashTable::setPointer(const std::string &key, void *pv) {
    int pos = findPos(key);

    if (pos != -1) {
        data[pos].pv = pv;
        return 0;
    }
    else {
        return 1;
    }
}

// lazily delete item with specified key 
bool hashTable::remove(const std::string &key) {
    int pos = findPos(key);

    if (pos != -1) {
        data[pos].isDeleted = true;
        return true;
    }
    else {
        return false;
    }
}
