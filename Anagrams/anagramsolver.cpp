/*HEADER:
 *Course: CS106B
 *Year: 2018
 *Name: Chase Renick
 *Assignment: To construct anagrams from a phrase passed by the user based on words found in a passed dictionary. The maximum number of Anagrams to be displayed
 *            should be indicated by the user.
 */

//LIBRARIES
#include <string>
#include "letterinventory.h"
#include "set.h"
using namespace std;


//FUNCTION PROTOTYPES
void pruneDictionary(LetterInventory li, Set<string>& dictionary, Set<string>& small_dictionary);
void findAnagramsHelper(LetterInventory li, int max, Set<string>&dictionary, Vector<Vector<string>>& anagram_inventory);
void buildBackAnagram(string& word, Vector<Vector<string>>& new_vector,Vector<Vector<string>>& anagram_inventory);
int printAnagrams(Vector<Vector<string>>anagram_inventory, int max);
int findAnagrams(string phrase, int max, Set<string>& dictionary);


//MAIN FUNCTIONS

/*Function: The main function that is not to be changed.
 *Input: A string or phrase to be selected, the maximum number of anagrams to be passed, and a dictionary to see what anagrams can be formed
 *Output: An integer indicating the number of anagrams found in the phrase based on the dictionary
 */
int findAnagrams(string phrase, int max, Set<string>& dictionary) {
    LetterInventory li (phrase);
    Vector<Vector<string>> anagram_inventory;

    findAnagramsHelper(li,max, dictionary, anagram_inventory);
    int num_of_anagrams = printAnagrams(anagram_inventory, max);
    cout << "\n";
    return num_of_anagrams;
}


/*Function: Goes through all the found anagrams and finds that subset that equals what was specified as the max number of words allowed.  For the moment, this
 * approach is not the most efficient since I am looping through all identified anagrams but only outputting a subset that make up max number of words
 *Input: A vector<vector<string>> that contains all the anagrams found through the findAnagramsHelper function
 *Output: Each of the anagram combinations printed on a new line
 */
int printAnagrams(Vector<Vector<string>>anagram_inventory, int max){
    int count;
    count = 0;
    for (int i=0; i<anagram_inventory.size(); ++i){
        if (anagram_inventory[i].size()==max){
            cout << anagram_inventory[i]<< endl;
            count +=1;
        }
    }
    return count;
}


/*Function: Prune's the dictionary a second time by checking whether a given letter inventory contains the word and passing only those words which are contained
 *          in the dictionary to a smaller dictionary.
 *Input: The letter inventory, the macro dictionary, and an empty micro dictionary
 *Output: Nothing -> it is a void function, but the rsult is a smaller dictionary that will contain only words that can make up those in the letter inventory
 */
void pruneDictionary(LetterInventory li, Set<string>& dictionary, Set<string>& small_dictionary){
    for (string word:dictionary){
        if (li.contains(word)){
            small_dictionary.add(word);
        }
    }
}


/*Function: Loops through vector vector of strings in new vector and adds vectors back anagram_inventory to keep track of words that make up the anagram
 *Input: Word, a Vector<Vector<string>> new vector, and anagram inventory
 *Output: Adds back other anagram words to anagram inventory
 */
void buildBackAnagram(string& word, Vector<Vector<string>>& new_vector,Vector<Vector<string>>& anagram_inventory){
    for (int i=0; i<new_vector.size(); ++i)
    {
        Vector<string> temp_word_vector;
        temp_word_vector.clear();
        temp_word_vector.add(word);
        anagram_inventory.add(temp_word_vector + new_vector[i]);
    }
}


/*Function: When letter inventory is empty or the new vector is of size 0, this function will add words back to a temp vector, and that vector to
 *          anagram inventory
 *Input:The letter inventory (phrase), the word, the temp vector, the new vector, and anagram inventory
 *Output: Nothing -> it is a void funciton, but it adds the temporary vector back to the anagram inventory
 */
void addWordBeforeBase(LetterInventory& li,string word, Vector<string>& temp, Vector<Vector<string>>& new_vector,Vector<Vector<string>>& anagram_inventory){
    if (li.isEmpty() && new_vector.size()==0)
    {
        temp.clear();
        temp.add(word);
        anagram_inventory.add(temp);
    }
}


/*Function: findAnagramHelper recursively looks for what words can be found in the letter inventory based on the dictionary passed, and keeps track of various anagrams
 *          through each call in anagram_inventory
 *Input: A passed letter inventory, a integer of the maximum number of anagrams, a dictionary, and an inventory of all found anagrams, which is intially empty
 *Output: Noting -> it is a void function, but it should keep track of the number of found anagrams as it recursively finds
 */
void findAnagramsHelper(LetterInventory li, int max, Set<string>&dictionary, Vector<Vector<string>>& anagram_inventory){
    //prune dictionary here so that when it is passed in later on into the recursive call it can be smaller
    Set<string> small_dictionary;
    pruneDictionary(li,dictionary,small_dictionary);

    //initialize
    Vector<Vector<string>> new_vector;
    Vector<string> temp;

    if (li.isEmpty()){
        //base case checks to see if letter inventory is empty
     return;
    }
    else if (max<0) {
        //an exception to take care of edge cases such as the max # of anagrams being < 0
        throw "You passed a negative value for int. Please revise your response.";
    }else if (max==0 || max > 1){
        //choose -> Part 1 of choose, explore, unchoose
        for (string word: small_dictionary){
            //start going through each word in the pruned dictionary
            if (li.contains(word)){
                new_vector.clear();
                //remove the found word from the letters contained in letter inventory
                li.subtract(word);
                //recursively repeat this elmination process with a smaller letter inventory, a smaller dictionary, same max, and new vector till we reach base case
                findAnagramsHelper(li, max, small_dictionary, new_vector);            
                //function loops vector vector of strings in new vector and adds those back to anagram inventory
                buildBackAnagram(word,new_vector,anagram_inventory);
                //function relates to base case and adding last word back to anagram inventory
                addWordBeforeBase(li, word, temp, new_vector, anagram_inventory);
                //unchoose the word, or in other words put it back in the letter inventory to go down other sections of the tree
                li.add(word);
             }
        }
    }
}



