/*
 * CS 106B/X String Recursion Problems
 * This client program contains a text menu for running your string functions.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2018/10/11
 * - 18au 106B initial version
 * @version 2018/01/25
 * - 18wi 106B initial version
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "stringproblems.h"

// function prototype declarations
void test_contains();
void test_isBalanced();
void test_lettersOnly();
void test_printBinary();

int main() {
    std::cout << "Welcome to CS 106B/X String Problems!" << std::endl;

    std::string choice;
    do {
        std::cout << std::endl;
        choice = toLowerCase(trim(getLine(
                "l)ettersOnly "
                "i)sBalanced "
                "p)rintBinary "
                "c)ontains "
                "q)uit?")));
        if (choice == "l") {
            test_lettersOnly();
        } else if (choice == "i") {
            test_isBalanced();
        } else if (choice == "p") {
            test_printBinary();
        } else if (choice == "c") {
            test_contains();
        }
    } while (choice != "q");

    std::cout << std::endl;
    std::cout << "Have a nice day!" << std::endl;
    return 0;
}

void test_contains() {
    std::string s1 = getLine("String 1 (haystack)? ");
    std::string s2 = getLine("String 2 (needle)  ? ");
    bool result = contains(s1, s2);
    std::cout << "Returned: " << std::boolalpha << result << std::endl;
}

void test_lettersOnly() {
    std::string s = getLine("String to process? ");
    std::string result = lettersOnly(s);
    std::cout << "Returned: \"" << result << "\" "
              << "(length " << result.length() << ")" << std::endl;
}

void test_printBinary() {
    int n = getInteger("Number to convert to binary? ");
    std::cout << n << " in binary is: ";
    std::cout.flush();
    printBinary(n);
    std::cout << std::endl;
}

void test_isBalanced() {
    std::string s = getLine("String to check? ");
    bool result = isBalanced(s);
    if (result) {
        std::cout << "Yes, that string is balanced." << std::endl;
    } else {
        std::cout << "No, that string is not balanced." << std::endl;
    }
}
