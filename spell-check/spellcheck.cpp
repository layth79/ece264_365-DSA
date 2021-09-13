/*
ECE-365 Project 1: spell-checker
Layth Yassin
Professor Sable

This file creates the spell-checking program
DESCRIPTION
*/

#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>

const int MAX = 26;
 
// Returns a string of random alphabets of length n
std::string printRandomString(int n)
{
    char alphabet[MAX] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z' };
 
    std::string res = "";
    for (int i = 0; i < n; i++)
        res = res + alphabet[rand() % MAX];
     
    return res;
}

int main() {
    std::string randoms[100000];
    hashTable ht;
    ht.insert("hello");
    ht.insert("my");
    ht.insert("name");
    if (ht.contains("hello"))
        std::cout << "found randoms[25]" << std::endl;
    for (int i = 0; i < 1000000; i++) {
        std::string temp = printRandomString(6);
        if (i < 100000)
            randoms[i] = temp;
        ht.insert(temp);
    }

    if (ht.contains(randoms[25]))
        std::cout << "found randoms[25]" << std::endl;
    if (ht.contains(randoms[2500]))
        std::cout << "found randoms[2500]" << std::endl;
    if (ht.contains(randoms[47865]))
        std::cout << "found randoms[47865]" << std::endl;
    if (ht.contains(randoms[99999]))
        std::cout << "found randoms[99999]" << std::endl;
    if (ht.contains("kalb"))
        std::cout << "wrongly identified kalb" << std::endl;

    //std::string dictFileName, checkedFileName, outputFileName;
    //std::cout << "Enter the name of the dictionary file: ";
    //std::cin >> dictFileName;
    //std::cout << "Enter the name of the document file to be spell-checked: ";
    //std::cin >> checkedFileName;
    //std::cout << "Enter the name of the file where the output will be written: ";
    //std::cin >> outputFileName;
    std::cout<<"end"<<std::endl;
    return 0;
}
