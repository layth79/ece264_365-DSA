/*
ECE-365 Project 2: useHeap
Layth Yassin
Professor Sable

This file includes implementations of functions from heap class.
*/

#include "heap.h"
#include <climits>

// constructor to initialize heap
heap::heap(int capacity) {
    this->capacity = capacity;
    currentSize = 0;
    hashTable mapping(capacity * 2);
    data.resize(capacity + 1);
}

// inserts a new node into the binary heap
int heap::insert(const std::string &id, int key, void *pv) {
    if (currentSize < capacity) {
        if (mapping.contains(id)) {
            return 2;
        }
        else {
            ++currentSize;
            data[currentSize].id = id;
            data[currentSize].key = key;
            data[currentSize].pData = pv;
            mapping.insert(id, &data[currentSize]);
            percolateUp(currentSize);

            return 0;
        }
    }
    else {
        return 1;
    }
}

// sets the key of the specified node to the specified value
int heap::setKey(const std::string &id, int key) {
    if (mapping.contains(id)) {
        node *pn = static_cast<node *> (mapping.getPointer(id));
        int oldKey = pn->key; // store old key to decide what kind of percolation to do
        pn->key = key;

        if (oldKey < key) {
            percolateDown(getPos(pn));
        }
        else if (oldKey > key) {
            percolateUp(getPos(pn));
        }

        return 0;
    }
    else {
        return 1;
    }
}

// returns data with smallest key and deletes node from heap
int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    if (currentSize > 0) {
        if (pId != nullptr) {
            *pId = data[1].id;
        }
        if (pKey != nullptr) {
            *pKey = data[1].key;
        }
        if (ppData != nullptr) {
            *(static_cast<void **> (ppData)) = data[1].pData;
        }

        mapping.remove(data[1].id); // remove first element in heap containing minimum key
        data[1] = std::move(data[currentSize--]); // move last node of heap to beginning and percolate down
        percolateDown(1); 

        return 0;
    }
    else {
        return 1;
    }
}

// deletes the node with the specified id from the heap
int heap::remove(const std::string &id, int *pKey, void *ppData) {
    if (mapping.contains(id)) {
        node *pn = static_cast<node *> (mapping.getPointer(id));
        int pos = getPos(pn);
        int oldKey = pn->key;

        if (pKey != nullptr) {
            *pKey = pn->key;
        }
        if (ppData != nullptr) {
            *(static_cast<void **> (ppData)) = pn->pData;
        }

        mapping.remove(id);
        *pn = data[currentSize];
        int currentKey = pn->key;
        --currentSize;

        if (currentKey < oldKey) {
            percolateUp(pos);
        }
        else if (currentKey > oldKey) {
            percolateDown(pos);
        }

        return 0;
    }
    else {
        return 1;
    }
}

// percolates element up the heap until the correct location is found 
void heap::percolateUp(int posCur) {
    node copy = data[posCur];

    for (; (posCur > 1) && (copy.key < data[posCur / 2].key); posCur /= 2) {
        data[posCur] = std::move(data[posCur / 2]);
        mapping.setPointer(data[posCur / 2].id, &data[posCur / 2]);
    }

    data[posCur] = std::move(copy);
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

// percolates element down the heap until the correct location is found 
void heap::percolateDown(int posCur) {
    node copy = data[posCur];
    int child;    

    for (; posCur * 2 <= currentSize; posCur = child) {
        child = 2 * posCur;
        if (child != currentSize && data[child + 1].key < data[child].key) {
            ++child;
        }
        if (data[child].key < copy.key) {
            data[posCur] = std::move(data[child]);
            mapping.setPointer(data[posCur].id, &data[posCur]);
        }
        else {
            break;
        }
    }

    data[posCur] = std::move(copy);
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

// returns position of given node
int heap::getPos(node *pn) {
    int pos = pn - &data[0];
    return pos;
}
