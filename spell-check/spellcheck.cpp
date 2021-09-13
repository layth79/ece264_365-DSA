/*
ECE-365 Project 1: spell-checker
Layth Yassin
Professor Sable

This file creates the spell-checking program

This program is an efficient implementation of a spell-checker that makes use of a hash table.
It takes in 3 files: a dictionary file, a spell-checking file, and an output file.
The dictionary contains the correct spelling of every word that is "known". If a word
is not in the dictionary file, it is considered an "unknown" word. The dictionary file
contains one word per line and is loaded into a hash table. The spell-checking file contains
the document that will be spell-checked. The program iterates through the spell-checking
document. If a word is greater than 20 characters, the program will write to the output
file that the word is long along with its line number. If the word is in the dictionary,
meaning it is a known word, the program writes nothing to the output file. If the word
is not in the dictionary, then the program writes to the output file that the word is unknown
along with its line number in the document.
*/

#include "hash.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// checks if a digit is in a given word
inline bool digitInWord(std::string &word) {
    for (char c : word) {
        if (isdigit(c)) {
            return true;
        }
    }
    return false;
}

double getCpuTime() {
    return (double) clock() / CLOCKS_PER_SEC;
}

int main() {
    // initialization of the hashTable object that will be used to store the dictionary
    hashTable ht;

    std::ifstream dict, check;
    std::ofstream output;
    std::string dictFileName, checkFileName, outputFileName, dictWord, checkLine;

    // prompt user for dictionary file    
    std::cout << "Enter the name of the dictionary file: ";
    std::cin >> dictFileName;
    dict.open(dictFileName);

    // start measuring time for loading in dictionary into hash table
    double start = getCpuTime();
    
    // loop through dictionary file line by line, which implies word by word, and insert into hash table
    while (std::getline(dict, dictWord)) {
        transform(dictWord.begin(), dictWord.end(), dictWord.begin(), ::tolower);
        ht.insert(dictWord);
    }

    // end measuring time for loading in dictionary into hash table, and compute time
    double end = getCpuTime();
    double dictTime = end - start;
    std::cout << "Total time (in seconds) to load dictionary: " << dictTime << std::endl;

    // prompt user for document file to be spell-checked
    std::cout << "Enter the name of the document file to be spell-checked: ";
    std::cin >> checkFileName;
    check.open(checkFileName);

    // prompt user for output file where output will be written
    std::cout << "Enter the name of the file where the output will be written: ";
    std::cin >> outputFileName;
    output.open(outputFileName);

    // start measuring time for spell-checking the document
    start = getCpuTime();

    int lineNumber = 1; // keeps track of what file line number it is up to
    // iterate line by line through spell-checking document
    while (std::getline(check, checkLine)) {
        // convert line to all lowercase
        transform(checkLine.begin(), checkLine.end(), checkLine.begin(), ::tolower);
        
        // initialize an array where all the words in current line number will be stored
        std::string words[checkLine.length()];
        
        int i = 0;
        // iterate through line char by char and storing words in the "words" array
        for (char c : checkLine) {
            if (isalpha(c) || isdigit(c) || c == '-' || c == '\'') {
                words[i].push_back(c);
            }
            else {
                i++;
            }
        }

        // iterate through words array to determine which words are too long or not defined in dictionary 
        for (std::string word : words) {
            // checks if word is too long
            if (word.length() > 20) {
                output << "Long word at line " << lineNumber << ", starts: " << word.substr(0, 20) << std::endl;
            }
            // checks if word element stored is empty or has a digit; if so, ignore that word
            else if (word == "" || digitInWord(word)) {
                continue;
            }
            // otherwise the word element is a valid word; check if it is in the dictionary 
            else if (!ht.contains(word)) {
                output << "Unknown word at line " << lineNumber << ": " << word << std::endl;
            }
        }
        lineNumber++;
    }

    // end measuring time for spell-checking document, and compute time
    end = getCpuTime();
    double checkTime = end - start;
    std::cout << "Total time (in seconds) to check document: " << checkTime << std::endl;

    // close all files that were opened
    dict.close();
    check.close();
    output.close();

    return 0;
}
