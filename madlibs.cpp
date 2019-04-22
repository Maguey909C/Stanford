/* HEADER:
 * Course: CS106B
 * Year: 2018
 * Name: Chase Renick
 * Assignment Description: Build a program that can read in mad lib text files, and allow users to write their own madlibs in the process.
 * The program should also be able to keep score of how many substitutions were made as well as how the user progresses
 */

//LIBRARIES:
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include <vector>
using namespace std;

//FUNCTION PROTOTYPES:
void introduction ();
int analysis(ifstream &infile, string&name_of_file );
void final_stats(const int&x, const int&y);

//MAIN
int main() {
    ifstream input;
    string name_of_file;
    string answer;
    int num_of_stories = 0;
    int total_placeholders = 0;

    vector<string> all_files = {"clothing.txt","marty.txt","mymadlib.txt", "tarzan.txt", "torture1.txt","torture2.txt","university.txt","victoria.txt","wannabe.txt"};

    introduction();

    while (true) {
        cout << "Mad Lib Input File? "; cin >> name_of_file;
        if(find(all_files.begin(), all_files.end(), name_of_file) != all_files.end()) {
            cout << "\n";
            total_placeholders += analysis(input, name_of_file);
            num_of_stories += 1;
            while(true) {
                ifstream input;
                cout << "Do another Mad Lib (Y/N)? "; cin >> answer;
                if (answer == "Y" or answer=="y"){
                    cout << "Mad Lib Input File? "; cin >> name_of_file;
                    cout << "\n";
                    total_placeholders += analysis(input, name_of_file);
                    num_of_stories += 1;
                    cout << "Do another Mad Lib (Y/N)? "; cin >> answer;
                    if (answer == "Y" or answer=="y"){
                        break;
                    }else if (answer == "N" or answer == "n") {
                        cout << "\n";
                        final_stats(num_of_stories, total_placeholders);
                        input.close();
                        return 0;
                    }else{
                        cout << "\n";
                        final_stats(num_of_stories, total_placeholders);
                        input.close();
                        return 0;
                    }
                } else if (answer == "N" or answer == "n") {
                    cout << "\n";
                    final_stats(num_of_stories, total_placeholders);
                    input.close();
                    return 0;
                } else {
                    cout << "Please type a word that starts with 'Y' or 'N'." << endl;
                }
            }
        } else {
            cout << "Unable to open that file. Try again." << endl;
        }
    }
}


/*Function: Introduces the Mad Lib Game
 * INPUT: NONE
 * OUTPUT: Lines that introduce the game
 */
void introduction () {
    cout << "Welcome to CS 106B Mad Libs!" << endl;
    cout << "I will ask you to provide various words" << endl;
    cout << "and phrases to fill in a story." << endl;
    cout << "At the end, I will display your story to you." << endl;
    cout << " " << endl;
}

/*Fuction: Introduces the Mad Lib Game
 * INPUT: An ifstream, and the name of a text file
 * OUTPUT: A new version of the text file with various words replaced based on characters "<" and ">"  as well as how many were replaced
 * NOTE: I am aware this function performs too many operations and thus is not ideal in terms of procedural decomposition, but my C++ is still growing.
 */
int analysis(ifstream &infile, string&name_of_file ) {

    string line;
    string user_word;
    int place_holder = 0;

    vector<string> user_inputs;

    infile.open(name_of_file);
    while (getline(infile,line)){

    //On a per line basis, I am looking for how many words will have to be replaced and counting them up to later iterate on them
        int count = 0;
        char d = '<';
        for (int i = 0; i < line.size(); i++){
            if (line[i] == d)
            {
                ++ count;
            }
        }
    //If there are not words to be replaced in the line then I want to just push the line back and store it as it is
    //Otherwise I want to iterate over the line starting with the first position of "<", getting the word cleaned up for the user to read it, then replacing the word, and saving it as a
    //new line which will account for all the subsitutions.
        if (count == 0) {
            user_inputs.push_back(line);
        } else{
        for (int j = 0; j < count; j++){
            unsigned first_lessthan_pos = line.find_first_of("<");
            unsigned first_greaterthan_pos = line.find_first_of(">");

            string one_word_per_line = line.substr(first_lessthan_pos, first_greaterthan_pos-first_lessthan_pos+1);
            string cleaned_word;
            cleaned_word = toLowerCase(stringReplace(stringReplace(stringReplace(one_word_per_line,"<",""),">",""),"!",""));
            if (cleaned_word[0] == 'a' or cleaned_word[0]== 'e' or cleaned_word[0] == 'i' or cleaned_word[0] == 'o' or cleaned_word[0] == 'u'){
                    cout << "Please type the name of an " << cleaned_word <<": "; cin >> user_word;
                } else {
                    cout << "Please type the name of a " << cleaned_word <<": "; cin >> user_word;
                }
            place_holder+=1;
            string new_line = line.replace(first_lessthan_pos,first_greaterthan_pos-first_lessthan_pos+1, user_word);
//            string new_line = stringReplace(line,one_word_per_line,user_word);
            line = new_line;
            if (j==count-1){
                user_inputs.push_back(new_line);
            }
        }
      }
    }
    //When all the subsitutions have been made then I want to read back the lines that were in the vector to show the edited version of the story
    cout << "" << endl;
    for (string i: user_inputs){
        cout << i << endl;
        }
    //Finally, I will let the user know how many times we made a subsitution
    cout << "" << endl;
    cout << place_holder << " place_holder(s) replaced" << endl;
    return place_holder;
  }

/*Fuction: Introduces the Mad Lib Game
 * INPUT: Number of times you played a madlib game, total number of words replaced
 * OUTPUT: Summary stats of how many games you played, total replacements, and an average
 */
void final_stats(const int&x, const int&y){
    double average = (y/x);
    cout << "Mad Lib stories you created: " << x << endl;
    cout << "Total placeholders replaced: " << y << " ("<< average << " per story)" << endl;}
