/*
ECE-365 Project 4: dynamic programming
Layth Yassin
Professor Sable

A "merge" of two strings is defined as a third string that contains all the characters
from each of the original two strings mixed together, where the characters can be
interspersed, but in their original order. This program determines whether a string
is a valid merge of the other two given strings. The program uses bottom-up dynamic
programming to solve the problem.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>

#define word1Max 1001
#define word2Max 1001

int mat[word1Max][word2Max];

bool validMerge(std::string word1, std::string word2, std::string &mergeWord);

int main() {
    std::string inputFileName, outputFileName;
    std::ifstream input;
    std::ofstream output;

    // prompt user for input file
    std::cout << "Enter name of input file: ";
    std::cin >> inputFileName;
    input.open(inputFileName);

    // prompt user for output file
    std::cout << "Enter name of output file: ";
    std::cin >> outputFileName;
    output.open(outputFileName);

    std::string word1, word2, mergeWord;
    // parse input file
    while (input >> word1 >> word2 >> mergeWord) {
        // set all matrix entries to -1 before checking each merge
        for (int i = 0; i < word1Max; i++) {
            for (int j = 0; j < word2Max; j++) {
                mat[i][j] = -1;
            }
        }

        // perform algo to determine if mergeWord is a valid merge
        bool isValidMerge = validMerge(word1, word2, mergeWord);
        if (((word1.length() + word2.length()) != mergeWord.length()) || !isValidMerge) {
            output << "*** NOT A MERGE ***" << std::endl;
        }
        else {
            output << mergeWord << std::endl;
        }        
    }

    return 0;
}

bool validMerge(std::string word1, std::string word2, std::string &mergeWord) {
    // increment first possible parts of valid paths
    if (word1[0] == mergeWord[0]) {
        mat[1][0]++;
    }
    if (word2[0] == mergeWord[0]) {
        mat[0][1]++;
    }

    int word1Len = word1.length();
    int word2Len = word2.length();

    // populate the matrix
    for (int i = 0; i < word1Len + 1; i++) {
        for (int j = 0; j < word2Len + 1; j++) {
            if (mat[i][j] != -1) {
                if (mergeWord[mat[i][j] + 1] == word1[i]) {
                    mat[i + 1][j] = mat[i][j] + 1;
                }
                if (mergeWord[mat[i][j] + 1] == word2[j]) {
                    mat[i][j + 1] = mat[i][j] + 1;
                }
            }
        }
    }

    // check if matrix indicates if merge is valid or not
    if (mat[word1Len][word2Len] != -1) {
        // merge is valid, capitalize necessary letters in mergeWord
        int idx = mergeWord.length() - 1;
        while (idx >= 0) {
            idx--;
            if (mat[word1Len][word2Len - 1] == idx && word2Len > 0) {
                word2Len--;
            }
            else {
                mergeWord[idx + 1] = toupper(mergeWord[idx + 1]);
                word1Len--;
            }
        }
        return true;
    }
    
    // merge is invalid since cell at index (word1Len, word2Len) was never incremented
    return false;
}
