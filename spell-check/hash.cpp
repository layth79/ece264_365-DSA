/*
ECE-365 Project 1: Spell Checker
Layth Yassin
Professor Sable

This file includes implementations of functions from hashTable class
*/

#include "hash.h"
#include <iostream>

#define primeNumbersLength sizeof(primeNumbers)/sizeof(primeNumbers[0])

// good precomputed prime numbers to use for resizing the hash table
int primeNumbers[] = {98317, 393241, 1572869, 6291469, 25165843, 100663319, 402653189, 1610612741};

// constructor to initialize hash table
hashTable::hashTable(int size) {
    capacity = getPrime(size);
    data.resize(capacity);
    filled = 0;
    for (int i = 0; i < capacity; i++) {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
        data[i].pv = nullptr;
    }
}

// inserts given key into hash table
int hashTable::insert(const std::string &key, void *pv) {
    // if load factor exceeds 0.5, perform rehash
    double loadFactor = (double) filled/capacity;
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

    while (data[probe].isOccupied) {
        if (probe == capacity - 1) {
            probe = 0;
        }
        else {
            probe++;
        }
    }
    
    data[probe].key = key;
    data[probe].isOccupied = true;
    data[probe].isDeleted = false;
    data[probe].pv = pv;
    filled++;
    return 0;

    /* // if load factor exceeds 0.5, perform rehash
    double loadFactor = (double) filled/capacity;
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

    if (data[hashIndex].isOccupied == false || data[hashIndex].isDeleted == true) {
        probe = -1;
    }
    else {
        if (hashIndex == capacity - 1) {
            probe = 0;
        }
        else {
            probe = hashIndex + 1;
        }
    }

    while ((probe != hashIndex) && (probe != -1)) {
        if (data[probe].isOccupied == false || data[probe].isDeleted == true) {
            probe = -1;
        }
        else {
            if (probe == capacity - 1) {
                probe = 0;
            }
            else {
                probe++;
            }
        }
    }
    if (probe != -1) {
        hashIndex = probe;
    }
    data[hashIndex].key = key;
    data[hashIndex].isOccupied = true;
    data[hashIndex].isDeleted = false;
    data[hashIndex].pv = pv;
    filled++;
    return 0; */
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

// hash function, code based on https://cp-algorithms.com/string/string-hashing.html
int hashTable::hash(const std::string &key) {
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
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
    int probe = hashIndex + 1;

    if (data[hashIndex].isOccupied == false) {
        return -1;
    }
    else if (data[hashIndex].key == key) {
        return hashIndex;
    }

    while (probe != hashIndex) {
        if (data[probe].isOccupied == false) {
            return -1;
        }
        else if (data[probe].key == key) {
            return probe;
        }
        else {
            if (probe == capacity - 1) {
                probe = 0;
            }
            else {
                probe++;
            }
        }
    }
}

// makes hash table bigger, returns true on success and false otherwise
bool hashTable::rehash() {
    std::vector<hashItem> oldVect = data;
    int newCapacity = getPrime(2 * capacity);
    try {
        data.resize(newCapacity);
    }
    catch (std::bad_alloc) {
        return false;
    }
    capacity = newCapacity;
    filled = 0;

    for (int i = 0; i < newCapacity; i++) {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
        data[i].pv = nullptr;
    }
    for (int i = 0; i < oldVect.size(); i++) {
        if ((oldVect[i].isOccupied == true) && (oldVect[i].isDeleted == false)) {
            insert(oldVect[i].key);
        }
    }
    return true;
}

// returns prime number to serve as size of hash table
unsigned int hashTable::getPrime(int size) {
    for (int i = 0; i < primeNumbersLength; i++) {
        if (size <= primeNumbers[i]) {
            return primeNumbers[i];
        }
    }
    return primeNumbers[primeNumbersLength - 1];
}
