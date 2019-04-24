/*HEADER:
 *Course: CS106B
 *Year: 2018
 *Name: Chase Renick
 *Assignment: Grammar Solver, designed to generate text by recursively selecting strings and grammar rules
 */

//LIBRARIES
#include "grammarsolver.h"
#include "map.h"
#include "console.h"
#include <strlib.h>
#include <istream>
#include <iostream>
#include <vector>
using namespace std;

//FUCTION PROTOTYPES
Map<string, vector<string>> make_map(istream& input);
vector<string> split_and_choose(vector<string>& small_vector);
void recursive_call(string&answers,Map<string, vector<string>>grammar_rules, string&symbol);
void through_each(string&answer,vector<string>&options, Map<string, vector<string>>grammar_rules);
void all_conditions(string&answer,vector<string> new_keys, Map<string, vector<string>>grammar_rules);

//FUNCTIONS

/*Functions: Takes in original text, calls a function to make a map of all word combinations, and pushes them back to an answer string
 *INPUT: A string of text, the symbol that denotes a particular grammar rule, and the number of times you want to repeat the process
 *OUTPUT: Returns a vector of possible answers, which are then looped and read by index on grammarmain.cpp
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    symbol = trim(symbol);
    Map<string, vector<string>> grammar_rules = make_map(input);
    string answer;
    vector <string> answers;
    for (int i=0; i < times; ++i){
        recursive_call(answer, grammar_rules, symbol);
        answers.push_back(answer);
        answer="";
    }
    return answers;
}

/*Function: Make a map (dictionary of all possible words or grammar rules based on input keys)
 *INPUT: List of text read line by line
 *OUTPUT: A map of possible combinations of key: value pairs
 */
Map<string, vector<string>> make_map(istream& input){
    string line;
    Map<string,vector<string> > grammar_rules;
    while (getline(input, line)){
        vector<string> first_part = stringSplit(line,"::=");
        vector<string> second_part = stringSplit(first_part[1],"|");
        grammar_rules.put(trim(first_part[0]),second_part);
    }
    return grammar_rules;
}

/*Function: Helper Function, randomly chooses a value from index, and splits on white spaces
 *INPUT: A vector, and the index in which to split on
 *RETURNS: A subset vector taken from an index
 */
vector<string> split_and_choose(vector<string>& small_vector){
    int random_choice = randomInteger(0,small_vector.size()-1);
    string selection = small_vector[random_choice];
    vector<string> split_vector = stringSplit(selection," ");
    return split_vector;
}

/*Function: Recursively traverses the possible grammar rules until it reaches a terminal point
 *Input: The empty answer to add to, the vector of possible options, the map of the choices
 *OUPUT: Void function that adds to the answer string through the process
 */
void through_each(string& answer,vector<string>& possible_options, Map<string, vector<string>>grammar_rules){
    for (string choice: possible_options){
        recursive_call(answer,grammar_rules,choice);
    }
}


/*Function: NOT SURE IF I SHOULD INCLUDE THIS FUNCTION OR IF IT MAKES IT LESS OBVIOUS I AM USING RECURISON LET ME KNOW YOUR THOUGHTS.
 *Input:
 *OUPUT:
 */
void all_conditions(string&answer,vector<string> new_keys, Map<string, vector<string>>grammar_rules){
    if (new_keys.size()==1){
        vector<string> two_options = stringSplit(new_keys[0]," ");
        through_each(answer, two_options, grammar_rules);

    }else if (new_keys.size()==0) {
        throw "The size of the string is empty, and it should not be.";

    }else{
        vector<string > split_vector = split_and_choose(new_keys);
        if (split_vector.size()>1){
            through_each(answer, split_vector, grammar_rules);
        }else{
            recursive_call(answer,grammar_rules,split_vector[0]);
        }
    }
}


/*Function: Recursively traverses the possible grammar rules until it reaches a terminal point
 *Input: An empty string, a map of possible keys with associated grammar rules, and the symbol to start recursive function
 *OUPUT: Void function that addss to the answer string through the process
 */
void recursive_call(string&answer,Map<string, vector<string>>grammar_rules, string&symbol){
    symbol = trim(symbol);
    if (grammar_rules.containsKey(symbol)){
        vector<string> new_keys = grammar_rules[symbol];
        all_conditions(answer,new_keys, grammar_rules);
    }else{
        answer+=symbol+" ";
    }
}
