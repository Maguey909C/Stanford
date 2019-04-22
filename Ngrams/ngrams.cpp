/*HEADER:
 *Course: CS106B
 *Year: 2018
 *Name: Chase Renick
 *Assignment: N-Grams, building a vehicle to spit out words that sound like a given string of text
 */

//LIBRARIES
#include <iostream>
#include "console.h"
#include <fstream>
#include <iomanip>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include <vector>
#include "random.h"
using namespace std;

//FUNCTION PROTOTYPES
void introduction();
void start_game(ifstream&infile, string&name_of_file, int&n_value);
void n_grams(ifstream&infile, string&name_of_file, int&n_value,int&response);
void random_word_generator(Map<string,string>&formul, string&random_start,int&response,int&n_value);
void read_vector(vector<string>&text);
int num_n_grams();
int num_random_words();
string give_random_key(Map<int,string>&selection_dic);
string random_value(vector<string>&value_vector);
string first_ngram_keys(vector<string>&other_n_gram, int&j, int&n_value);
string first_random_words(string&random_start);
string add_comma(string&final_subsets,int&n_value);
vector<string> break_string_to_vector(string&some_string);
string choose_words(Map<string,string>&formul,string&random_start);
Map<string,string> stuff(vector<string>&other_n_gram, int&n_value, Map<string,string>&formul);

int main() {
    ifstream input;
    string name_of_file;

    introduction();
    name_of_file = promptUserForFile("Input file name? ");
    int n_value;
    n_value = num_n_grams();
    cout << "\n";
    start_game(input, name_of_file, n_value);
    cout << "\n";
    cout << "Exiting." << endl;
    return 0;
}


/*Function:
 *INPUT:None
 *OUTPUT:Introduces the N-Gram game to user
 */
void introduction(){
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "\n";
}

/*Function:
 *INPUT: Input file stream, name of the file to perform n_gram, and the number of n grams
 *OUTPUT: Triggers functions to make n_gram dictionary and play the game
 */
void start_game(ifstream&infile, string&name_of_file, int&n_value){
    int response;
    while(true){
        response = num_random_words();
        if (response == 0){
            break;
        }else{
            n_grams(infile,name_of_file, n_value, response);
        }
    }
}

/*Function:
 *INPUT: Input, name of the file, number of n grams, and number of words user wants to see
 *OUTPUT:
 */
void n_grams(ifstream&infile, string&name_of_file, int&n_value,int&response){

    string word;
    Map<string, string> formul;
    Map<int,string> selection_dic;
    infile.open(name_of_file);

    int counter;
    counter = 0;

    vector<string> other_n_gram;

    while (infile >> word){
        counter += 1;
        other_n_gram.push_back(word);
        if (counter == n_value){
            string add_word;
            for (int j=0; j < other_n_gram.size(); ++j){

                string first_ngram_keys(vector<string>&other_n_gram, int&j, int&n_value);
                if (j < n_value-1){
                    add_word += other_n_gram[j];
                    if (j < n_value-2){
                        add_word+= ", ";
                    }

                }else{
                    formul.put(add_word, other_n_gram[counter-1]);
                    selection_dic.put(counter, add_word);
                    add_word = {};
                }
            }

        }else if (counter > n_value){
            string second_word;
            for (int j=n_value; j > 0; --j){
                if (j > 1){
                    second_word += other_n_gram[counter-j];
                    if (j > 2){
                        second_word += ", ";
                    }
                }else{
                    if (formul[second_word].size()>1){
                        formul[second_word] += ", " + other_n_gram[counter-j];
                        selection_dic.put(counter, second_word);
                    }else{
                        formul[second_word] += other_n_gram[counter-j];
                        selection_dic.put(counter, second_word);
                        second_word = {};
                    }
                }
            }
        }else{}
    }

  Map<string,string> wrap_map = stuff(other_n_gram, n_value, formul);
  string random_start = give_random_key(selection_dic);
  random_word_generator(wrap_map, random_start, response, n_value);
  infile.close();
  return;
}

//ALL OF THE FUNCTIONS IN THE PROGRAM /////////////////////////////////////

string first_ngram_keys(vector<string>&other_n_gram, int&j, int&n_value){
    string add_word;
    if (j < n_value-1){
        add_word += other_n_gram[j];
        if (j < n_value-2){
            add_word+= ", ";
            }
    }
    return add_word;
}

/*Function:
 *INPUT:
 *OUTPUT:
 */
string add_comma(string&final_subsets,int&n_value){
    if (stringSplit(final_subsets,",").size() < (n_value-1)){
        final_subsets += ", ";
    }
    return final_subsets;
}

/*Function: Wrap Around Map
 *INPUT: Vector of all words, number of n_grams, map of n_grams
 *OUTPUT: Returns a master map of all possible n_gram combinations
 */
Map<string,string> stuff(vector<string>&other_n_gram, int&n_value, Map<string,string>&formul){
    for (int p=0; p<n_value-1; ++p){
        string last_keys;
        string final_subsets;
        for (int u=-n_value+1+p; u<p; ++u){
            if (stringSplit(final_subsets,",").size() <= (n_value-1)){
                if (u==0){
                    final_subsets += other_n_gram[0];
                    final_subsets = add_comma(final_subsets,n_value);
//                    if (stringSplit(final_subsets,",").size() < (n_value-1)){
//                        final_subsets += ", ";
//                    }
                }else if (u>0){
                    final_subsets += other_n_gram[u];
                    final_subsets = final_subsets = add_comma(final_subsets,n_value);
//                    if (stringSplit(final_subsets,",").size() < (n_value-1)){
//                        final_subsets += ", ";
//                    }
                }else{
                final_subsets += other_n_gram[other_n_gram.size()+u];
                final_subsets = final_subsets = add_comma(final_subsets,n_value);
//                if (stringSplit(final_subsets,",").size() < (n_value-1)){
//                    final_subsets += ", ";
//                }
                }}
            else{
                break;
            }
        }
        formul[final_subsets] += other_n_gram[p];
        final_subsets ={};
        }
    return formul;
}


/*Function: Break Vectors Up
 *INPUT: Some string with commas as a delimiter
 *OUTPUT: Returns a vector of that string
 */
vector<string> break_string_to_vector(string&some_string){
    vector <string> new_string = stringSplit(some_string,",");
    return new_string;
}

/*Function: Select last value + add new one
 *INPUT: Takes in a vector and a new randomly chosen key from dictionary
 *OUTPUT: Returns next set of keys with "," in between
 */
string select_next_n_grams(vector<string>&other_vec, string&random_chosen_key){
    string first_word;
    first_word = trim(other_vec[other_vec.size()-1]);
    first_word += ", ";
    first_word += random_chosen_key;
    return first_word;
//    return trim(other_vec[other_vec.size()-1])+", "+random_chosen_key;
}

/*Function: Return the value based on vector
 *INPUT:Takes in a word vector
 *OUTPUT:Choose a random number based on size of vector and returns value associated with random index
 */
string choose_words(Map<string,string>&wrap_map,string&random_start){
    string keys = wrap_map[random_start];
    vector <string> new_vec = break_string_to_vector(keys);
    return random_value(new_vec);
}

/*Function: Choose a random word among available values
 *INPUT:Takes in a vector
 *OUTPUT:Selects a random index from vector and returns a string
 */
string random_value(vector<string>&value_vector){
    if (value_vector.size() == 1) {
        return trim(value_vector[0]);
    }else{
        return trim(value_vector[randomInteger(0,value_vector.size()-1)]);
    }
}

/*Function: Generate new n_gram text based on original file
 *INPUT:Takes in a word vector
 *OUTPUT:Choose a random number based on size of vector and returns value associated with random index
 */
void random_word_generator(Map<string,string>&wrap_map, string&random_start, int&response, int&n_value){
    vector<string> the_final_text;
//    the_final_text.push_back(first_random_words(random_start));
    vector <string> v = break_string_to_vector(random_start);
    for (string text: v){the_final_text.push_back(trim(text));}
    for (int i=0; i<response-n_value+1; ++i) {
        string random_chosen_key = choose_words(wrap_map,random_start);
        the_final_text.push_back(trim(random_chosen_key));
        vector <string> other_vec = break_string_to_vector(random_start);
        random_start = select_next_n_grams(other_vec,random_chosen_key);
    }
    cout << "\n";
    read_vector(the_final_text);
}


/*Function: Join all the strings together to form coherent stream
 *INPUT:Takes in a word vector
 *OUTPUT:Choose a random number based on size of vector and returns value associated with random index
 */
void read_vector(vector<string>&text){
    cout << "..." << stringJoin(text," ") << "..." << endl;
    cout << "\n";
}

/*Function: Choose a random key among available set
 *INPUT:Takes in a word vector
 *OUTPUT:Choose a random number based on size of vector and returns value associated with random index
 */
string give_random_key(Map<int,string>&selection_dic){
    int value = randomInteger(3,selection_dic.size());
    return selection_dic[value];
}

/*Function: Prompt user for the # of N_grams they want
 *INPUT: Nothing
 *OUTPUT: Prompts user to input a value for the number of n_grams
 */
int num_n_grams(){
    int n_gram_value;
    cout << "Value of N? "; cin >> n_gram_value;
    return n_gram_value;
}

/*Function: Prompt user for the # of words they want to see displayed
 *INPUT: Nothing
 *OUTPUT: Prompts the user to input a number to generate words from text, 0 will stop game
 */
int num_random_words(){
    int num;
    cout << "# of random words to generate (0 to quit)? " ; cin >> num;
    return num;
}
