/*
 CS 251 - Data Structures 
 Project 2 - Search Engine

 Author: Mariyam Haji 
 NetID: mhaji7

 System: Visual Studio Code on Windows 10
 
 This program uses the map and set abstractions to build a document search engine.
 The search engine finds webpages with body text that matches a user's query. 
 The body text of each page is pre-processed and stored as an inverted index 
 in a data structure that allows lightning-fast retrieval of search results. 

 search.h implements the functions and main.cpp calls and tests the functions.

 My creative component implements an option for building an inverted index without any stop words.
 Filtering out stop words productively reduces the size of the index and increases the accuracy of search results.
 The option is implemented in buildNoStopWordsIndex() with helper functions indexHelper1() and 
 indexHelper2() that are also used by buildIndex() as well as helper function retrieveStopWords().
 The default Snowball stoplist used in this program as "stopwords.txt" can be found at:
 https://github.com/igorbrigadir/stopwords/blob/master/en/snowball_original.txt
 
 TO RUN CREATIVE COMPONENT: Ensure "stopwords.txt" exists in the same folder. Uncomment line 217 of search.h in 
 searchEngine() and comment line 216. Then, make a call to searchEngine() in main.cpp. To change the stopwords 
 file, either add another file named "stopwords.txt" or change the file name on line 251 in retrieveStopWords().
*/


#include "search.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {

    /* TESTING cleanToken()
    cout << "\nTESTING cleanToken()\n" << endl;

    string answer;
    vector <string> returnHello { ".hello", "...hello", ".?.:hello", "...hello!!!", "...hELLo!!!" };
    vector <string> returnEmpty { "...!!!", ".235!", ".2 (35!" };
    vector <string> checkMiddle { "HE'LLO", "netID@uic.edu", " 12he! .llo  ,  " };

    answer = "hello";
    for (string s : returnHello) {
        cout << cleanToken(s) << " : " << (answer == cleanToken(s)) << endl;
    }

    answer = "";
    for (string s : returnEmpty) {
        cout << cleanToken(s) << " : " << (answer == cleanToken(s)) << endl;
    }

    // answers vary
    for (string s : checkMiddle) {
        cout << cleanToken(s) << endl;
    }

    */


    /* TESTING gatherTokens()
    cout << "\nTESTING gatherTokens()\n" << endl;

    vector <string> testTokens = { " 12he! .llo  ,  ", ".2 (35!", "to be or not to be", "EGGS! milk, fish,      @  bread cheese", 
                                   "mariyam", "mariyam mariyam mariyam hello" };

    for (string s : testTokens) {
        set <string> tokens = gatherTokens(s);
        cout << "# of tokens: " << tokens.size() << endl;
        for (string t : tokens) {
            cout << "'" << t << "'" << endl;
        }
    }

    */

   
    /* TESTING buildIndex() and findQueryMatches()
    cout << "\nTESTING buildIndex() and findQueryMatches()\n" << endl;

    map<string, set<string>> index;
    buildIndex("cplusplus.txt", index);         // tested with all provided files and in functions below

    // also tested the queries below by running "searchEngine("cplusplus.txt");"
    // and manually entering them in a productive sequence

    vector <string> searchQueries = 
    { "(i.e.,", "compare", "char", "   ,and", "member", "function",                 // single terms
      "class", "vector", "queue", "and",
      "member function", "function mariyam", "function function",                   // compound query - union
      "class class", "bool queue", "class queue", "vector bool",
      "class +class", "vector +bool", "bool +vector", "vector +", "vector +queue"   // compound query - intersection
      "vector bool -queue", "vector -bool +queue", "queue -class",                  // compound query - difference
      "queue -class +class", "and -vector",
      "+vector", "-vector", "-bool +vector", "-mariyam" "!!-class!!+ queue ",       // +/- in first term
      "mariyam", "vector -mariyam", "vector +mariyam", "vector bool mariyam",       // not found
      "vector bool +mariyam", "vector bool -mariyam",
      "queue", "QUEUE", "The", "the"                                                // case-insensitivity
    };       

    for (string query : searchQueries) {
        set<string> result = findQueryMatches(index, query);

        // added "cout << "\n" << wordInQuery;" to findQueryMatches()
        cout << "\n'" << query << "' (" << result.size() << ")\n";
        for (string thisURL : result) {
            cout << thisURL << endl;
        }
    }

    */


    /* TESTING searchEngine()
    cout << "\nTESTING searchEngine()\n" << endl;

    cout << "Enter filename: " << endl;
    string filename;
    cin >> filename;    
    searchEngine(filename);         // tested with all provided files and by manipulating functions above

    */


    /* TESTING buildNoStopWordsIndex()
    cout << "\nTESTING buildNoStopWordsIndex()\n" << endl;

    map<string, set<string>> indexNoStopWords;
    buildNoStopWordsIndex("cplusplus.txt", indexNoStopWords);

    vector <string> stopWords = retrieveStopWords();
    for (string word : stopWords) {
        set<string> result = findQueryMatches(indexNoStopWords, word);
        cout << "\n'" << word << "' (" << result.size() << ")\n";
    }

    // also used "searchEngine("cplusplus.txt");" to manually enter stop words and check output
    // also tested set operations using such queries: "vector", "vector and", "vector +and", "vector -and", etc.

    */
   

    searchEngine("cplusplus.txt");

    return 0;
}