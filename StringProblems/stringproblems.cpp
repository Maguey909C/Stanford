/*HEADER:
 *Course: CS106B
 *Year: 2018
 *Name: Chase Renick
 *Assignment: To complete mini recursive exercises that test knowledge of recursion among string and integers.
 */

//LIBRARIES TO INCLUDE
#include "stringproblems.h"
#include <string>
#include <cctype>
#include <iostream>
using namespace std;

//FUNCTION PROTOTYPES
bool helper_bracket(string& small_string,string& parentheses);
string contains_helper(string& lower_it);


//FUNCTIONS

/*Function: Splits a string into two parts, then recursively goes through each part until a single letter is reached, upon which the character is returned or
 * an empty string is returned. Function then adds these elements back to produce original string without white speces, blanks, or non-alpha numeric characters
 *INPUT: A string
 *OUTPUT: The string without white spaces, and removes non alpha numeric characters
 */
string lettersOnly(string s) {
      if (s.length()<=1){
          char final_character = s[0];
          if (isblank(final_character)==true || isspace(final_character)==true || isalnum(final_character)!=true){
              return "";
          }else{
              return s;
          }
      }else{
          int mid_point = (s.length()/2);
          string first_half = s.substr(0,mid_point);
          string second_half = s.substr(mid_point,s.length());
          string a = lettersOnly(first_half);
          string b = lettersOnly(second_half);

          return a + b;
      }
}

/*Function: A helper function for isBalanced function
 *INPUT: A string, and the characters that will be used in identiy whether or not to replace them with an empty string
 *OUTPUT: Returns a boolean, but with small string passed as reference it will be edited if brackets are found
 */
bool helper_bracket(string& small_string, string parentheses){
    int parentheses_index = small_string.find(parentheses);
    if (parentheses_index != string::npos){
        small_string = small_string.replace(parentheses_index,2,"");
        return true;
    }else{
        return false;
    }
}

/*Function: The function recursively looks at a string by calling a helper function that removes brackets passed in the helper function until on a equal pair
 * is found otherwise it will indicate that the brackets are not balanced.
 *INPUT: A string
 *OUTPUT: A boolean (True or False) as to whether or not the passed string is balanced
 */
bool isBalanced(string s) {
    if (s.empty()){
        return true;
    }else {
        if (helper_bracket(s,"()") || helper_bracket(s,"[]") || helper_bracket(s,"{}") || helper_bracket(s,"<>")){
            return isBalanced(s);
        }
    }
    return false;
}

/*Function: The function examines a given integer, halves it, looks at the quotient, and then recursively repeats the process until,
 * the quotient is 0. It will then express the original integer as 0's and 1's in binary. It also takes into account edge cases such as a negative
 * integer passed.
 *INPUT: Pass in an integer
 *OUTPUT: The integer expressed in binary (0's and 1's)
 */
void printBinary(int n) {
    int quotient = n / 2;
    int remainder = n % 2;

    if (quotient > 0)
    {
        printBinary(quotient);

    }else if (quotient<0){
        printBinary(quotient);
    }

    if (n > 0){
        cout << remainder;
    }else if (n<0) {
        cout << remainder*-1;
    }
}

/*Function: The function examines whether or not the second string can be found in the first string.  Fuction currently does not handle edge cases such as
 * case sensitivity.
 *INPUT: Two strings
 *OUTPUT: A boolean value that determines whether or not the second string passed in the function can be found in the first string
 */
bool contains(string s1, string s2) {
    //There is a better way of doing this...
    contains_helper(s1);
    contains_helper(s2);
    if (s1.length() < s2.length()){
        return false;
    }else if (s2.length()==0) {
        return true;
    }else{
        int word_1_length = s1.length();
        int word_2_length = s2.length();
        if (s1.substr(0,word_2_length) == s2){
            return true;
        }else{
            int next_s2_pos = word_2_length+word_2_length;
            s1 = s1.substr(word_2_length,next_s2_pos);
            contains(s1,s2);
        }
    }
        return true;
}

/*Function: Current function that recursively lowercases a given string
 *INPUT: A string
 *OUTPUT: A lower case version of that string
 */
string contains_helper(string& lower_it){

    for (int i=0; i<lower_it.size(); ++i){
        lower_it[i] = tolower(lower_it[i]);
    }
    return lower_it;
}
