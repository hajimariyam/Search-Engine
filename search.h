/*
 Project: Search Engine
 Author: Mariyam Haji 
 System: Visual Studio Code on Windows 10
 
 This program uses map and set abstractions to build a document search engine.
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
 
 TO RUN CREATIVE COMPONENT: Ensure "stopwords.txt" exists in the same folder. Uncomment line 214 of 
 search.h in searchEngine() and comment line 213. To change the stopwords file, either add another 
 file named "stopwords.txt" or change the file name on line 248 in retrieveStopWords().
*/


#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>    // find()
using namespace std;


/* Parameter: Whitespace-separated string of characters (token)
   Behavior: Clean token by trimming (replacing with "") any leading or trailing punctuation characters 
             (recognized via ispunct()) and convert each character to lowercase (via tolower()).
   Return value: "Cleaned" token, or empty string if the token contains no letters (assessed via isalpha()) 
*/
string cleanToken (string token) 
{
    // for leading punctuation
    while (ispunct(token[0])) {
        token.replace(0, 1, "");
    }

    // for trailing punctuation
    while (ispunct(token[token.size() - 1])) {
        token.replace(token.size()-1, 1, "");
    }

    int i = 0;
    bool containsALetter = false;
    while (token[i]) {
        if (!containsALetter && isalpha(token[i])) {
            containsALetter = true;
        }
        token[i] = tolower(token[i]);
        i++;
    }

    if (containsALetter) {
        return token;
    }
    
    return "";
}


/* Parameter: String containing the body text of a webpage
   Behavior: Tokenize (divide) the string by whitespace into individual token strings (words) using a stringstream. 
             Clean each token using cleanToken() and if non-empty after cleaning, store in a set to avoid duplicates.
   Return value: Set of unique, cleaned tokens 
*/
set<string> gatherTokens (string text) 
{
    set<string> tokens;
    
    string thisToken;
    stringstream myStringStream (text);
    while (getline (myStringStream, thisToken, ' ')) {
        thisToken = cleanToken(thisToken);
        if (thisToken != "") {
            tokens.insert(thisToken);
        }
    }

    return tokens;
}


/* Declarations of helper functions for buildIndex() and buildNoStopWordsIndex(). All are defined at the end of search.h */
vector <string> retrieveStopWords ();
int indexHelper1 (ifstream& fileStream, map<string, set<string>>& index, const vector <string>& stopWords);
void indexHelper2 (map<string, set<string>>& index, const vector <string>& stopWords, string webpageURL, string token);


/* For both buildIndex() and buildNoStopWordsIndex():
   Parameters: 1) name of file with webpage data, 2) map to be populated as an inverted index using the file data
   Return value: Count of webpages from the data file that were processed and added to the index 
*/

// Build an inverted index map that associates each token (key) with a set of URLs (value) where that token can be found.              
int buildIndex (string filename, map<string, set<string>>& index) 
{
    cout << "Stand by while building index...\n";    

    // empty vector, no stop words filtered in buildIndex()
    vector <string> stopWords;          // did not overload indexHelper1() instead to avoid 50+ lines of redundant code

    ifstream fileStream(filename);
    if (fileStream.fail()) { return 0; }
    
    int urlCounter = indexHelper1(fileStream, index, stopWords);
    
    fileStream.close();

    return urlCounter;
}

// Build an inverted index map that associates each token (key)--that is not a stop word--with a set of URLs (value) where that token can be found.
int buildNoStopWordsIndex (string filename, map<string, set<string>>& index) 
{
    cout << "Stand by while building index without stop words...\n";    

    // retrieve stop words
    vector <string> stopWords = retrieveStopWords();
    if (stopWords.size() == 0) {        // invalid file for stop words
        return 0;
    }

    // handle data file
    ifstream fileStream(filename);
    if (fileStream.fail()) { return 0; }

    int urlCounter = indexHelper1(fileStream, index, stopWords);
    
    fileStream.close();

    return urlCounter;
}


/* Declarations of helper functions for findQueryMatches(). All are defined at the end of search.h */
set<string> SetUnion (const set<string>& set1, const set<string>& set2);
set<string> SetIntersection (const set<string>& set1, const set<string>& set2);
set<string> SetDifference (const set<string>& set1, const set<string>& set2);


/* Parameters: 1) map abstraction populated as inverted index
               2) string search term that is non-empty and either a single word or multiple consecutive words, 
                  each of which (besides the first) may be preceded by '+' or '-' as the first character
   Behavior: Use cleanToken() to clean the search query and assume no query will clean to the empty string.
             For a single search term, matches are the URLs of the webpages containing that term.
             For a compound search query, process each word left to right:
             1) If prefaced with '-', remove matches for that term from existing results.
             2) If prefaced with '+', intersect matches for that term with existing results.
             3) Otherwise, union matches across search terms or existing results.
             The function returns appropriately if search query is not found in index.
   Return value: Set of matched URL strings 
*/
set<string> findQueryMatches (map<string, set<string>>& index, string query) 
{
    set<string> result;
    
    query = cleanToken(query);      // also ensures case-insensitive searching

    string wordInQuery;
    stringstream myStringStream (query);

    while (getline (myStringStream, wordInQuery, ' ')) {
        if (wordInQuery[0] == '-') {
            wordInQuery = cleanToken(wordInQuery);
            if (index.find(wordInQuery) != index.end()) {
                result = SetDifference(result, index.find(wordInQuery)->second);
            }   // else executes function return
        }

        else if (wordInQuery[0] == '+') {
            wordInQuery = cleanToken(wordInQuery);
            if (index.find(wordInQuery) != index.end()) {
                result = SetIntersection(result, index.find(wordInQuery)->second);
            }
            else {
                result.clear();     // if not in index, cannot intersect with anything
            }
        }

        else {
            wordInQuery = cleanToken(wordInQuery);
            if (index.find(wordInQuery) != index.end()) {
                result = SetUnion(result, index.find(wordInQuery)->second);
            }   // else executes function return
        }
    }

    return result;
}


/* Parameter: name of file with webpage data
   Behavior: Implement a console program that builds an inverted index from the file data using buildIndex(),
             prompts the user to enter a search query, finds matches for the query using findQueryMatches(),
             and prints the matched URLs. Exit the program when the user presses 'enter' (empty string).
             There are two options for the index: 1) buildIndex(), 2) buildNoStopWordsIndex
*/
void searchEngine (string filename) 
{
    map<string, set<string>> index;
    int totalWebpages = buildIndex (filename, index);
    // int totalWebpages = buildNoStopWordsIndex (filename, index);     // CREATIVE COMPONENT: uncomment to filter stop words in index

    cout << "Indexed " << totalWebpages << " pages containing " << index.size() << " unique terms" << endl;

    string userQuery;
    cout << "\nEnter query sentence (press enter to restart): ";
    getline (cin, userQuery);

    while (userQuery != "") {
        set<string> result = findQueryMatches(index, userQuery);

        cout << "Found " << result.size() << " matching pages" << endl;

        for (string thisURL : result) {
            cout << thisURL << endl;
        }

        cout << "\nEnter query sentence (press enter to restart): ";
        getline (cin, userQuery);
    }
    
    return;
}


// INDEX HELPER FUNCTIONS

/* Create and return a vector with each stop word read in from the file of stop words */
vector <string> retrieveStopWords () 
{   
    vector <string> stopWords;
    string thisStopWord;

    ifstream fileStream ("stopwords.txt");
    if (fileStream.fail()) { return stopWords; }

    while (getline(fileStream, thisStopWord)) {
        stopWords.push_back(thisStopWord);
    }

    fileStream.close();
    return stopWords;
}


/* Parameters: 1) file stream for file with webpage data; formatted in 2-line pairs per webpage: URL and body text
               2) map to be populated as an inverted index using the file data, 3) vector with stop words
   Behavior: Read each 2-line pair. Use gatherTokens() to extract the set of unique, cleaned tokens in each line of body text.
             Then, call indexHelper2() for each token in the set.
   Return value: Count of webpages from the data file that were processed and added to the index           
*/
int indexHelper1 (ifstream &fileStream, map<string, set<string>>& index, const vector <string>& stopWords) 
{
    int urlCounter = 0;
    string webpageURL;
    string webpageText;

    getline(fileStream, webpageURL);
    while (webpageURL != "") {
        urlCounter++;
        
        getline(fileStream, webpageText);
        set <string> setOfTokens = gatherTokens(webpageText);

        for (string token : setOfTokens) {
            indexHelper2(index, stopWords, webpageURL, token);
        }

        getline(fileStream, webpageURL);
    }

    return urlCounter;
}


/* Parameters: 1) map to be populated as an inverted index using the file data, 2) vector with stop words
               3) a URL from the data file, 4) a token from the body text associated to that URL
   Behavior: If the calling function for this function was buildIndex() or if it was buildNoStopWordsIndex() and the token is not a stop word:
             - if the token exists as a key in the map, add the URL to the set that is its value. 
             - otherwise, emplace the token as a key in the map and create a new set with the URL and set as the value. 
*/
void indexHelper2 (map<string, set<string>>& index, const vector <string>& stopWords, string webpageURL, string token) 
{
    if (stopWords.size() == 0 || 
        (stopWords.size() != 0  && (find(stopWords.begin(), stopWords.end(), token) == stopWords.end())))
    {
        if ( index.find(token) != index.end() ) {
            index.find(token)->second.insert(webpageURL);
        }
        else {
            set <string> setOfURLs;
            setOfURLs.insert(webpageURL);
            index.emplace(token, setOfURLs);
        }
    }
}


// SET HELPER FUNCTIONS

/* Create and return a new set that has every webpage URL from set1 and every webpage URL from set2 (' ') */
set<string> SetUnion (const set<string>& set1, const set<string>& set2) 
{
    set<string> result;
    for (string urlInSet1 : set1) {
        result.insert(urlInSet1);
    }
    for (string urlInSet2 : set2) {
        result.insert(urlInSet2);
    }
    return result;
}


/* Create and return a new set that has every webpage URL from set1 which is also in set2 ('+') */
set<string> SetIntersection (const set<string>& set1, const set<string>& set2) 
{
    set<string> result;
    for (string urlInSet1 : set1) {
        if (set2.find(urlInSet1) != set2.end()) {
            result.insert(urlInSet1);
        }
    }
    return result;
}


/* Create and return a new set that has every webpage URL from set1 which is not in set2 ('-') */
set<string> SetDifference (const set<string>& set1, const set<string>& set2) 
{
    set<string> result;
    for (string urlInSet1 : set1) {
        if (set2.find(urlInSet1) == set2.end()) {
            result.insert(urlInSet1);
        }
    }
    return result;
}
