//
//  CS 251 - Data Structures 
//  Project 2 - Search Engine
//
//  Author: Mariyam Haji 
//  NetID: mhaji7
//
//  System: Visual Studio Code on Windows 10
//  
//  This program uses the map and set abstractions to build a document search engine.
//  The search engine finds webpages with body text that matches a user's query. 
//  The body text of each page is pre-processed and stored as an inverted index 
//  in a data structure that allows lightning-fast retrieval of search results. 

//  search.h implements the functions and main.cpp calls and tests the functions.

//  My creative component is 
//  
//  
//  
//
//

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;


// Parameter: Whitespace-separated string of characters (token)
// Behavior: Clean token by trimming (replacing with "") any leading or trailing punctuation characters 
//           (recognized via ispunct()) and convert each character to lowercase (via tolower()).
// Return value: "Cleaned" token, or empty string if the token contains no letters (assessed via isalpha())
string cleanToken(string token) 
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


// Parameter: String containing the body text of a webpage
// Behavior: Tokenize (divide) the string by whitespace into individual token strings (words) using a stringstream. 
//           Clean each token using cleanToken() and if non-empty after cleaning, store in a set to avoid duplicates.
// Return value: Set of unique, cleaned tokens
set<string> gatherTokens(string text) 
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


int indexHelper1(ifstream &fileStream, map<string, set<string>>& index, vector <string> stopWords);

void indexHelper2(map<string, set<string>>& index, string webpageURL, string token);


// Parameters: 1) name of file with webpage data, 2) map to be populated as an inverted index using the file data
// Data files are formatted in 2-line pairs for each webpage as such: 1) URL, 2) body text
// Behavior: Use gatherTokens() to extract the set of unique, cleaned tokens in each line of body text.
//           If a token exists as a key in the map, add the URL to the set that is its value. 
//           Otherwise, emplace the token as a key in the map and create a new set with the URL and set as the value. 
// The inverted index map that associates each token (key) with a set of URLs (value) where that token can be found. 
// Return value: Count of webpages from the data file that were processed and added to the index
int buildIndex(string filename, map<string, set<string>>& index) 
{
    cout << "Stand by while building index...\n";    

    ifstream fileStream(filename);
    if (fileStream.fail()) {
        return 0;
    }

    vector <string> stopWords;      // expected to be empty
    int urlCounter = indexHelper1(fileStream, index, stopWords);
    
    fileStream.close();

    return urlCounter;
}


int buildNoStopWordsIndex (string filename, string fileStopWords, map<string, set<string>>& index) {
    cout << "Stand by while building index without stop words...\n";    

    // handle stop words
    ifstream fileStream1(fileStopWords);
    if (fileStream1.fail()) {
        return 0;
    }

    vector <string> stopWords;
    string thisStopWord;

    while (getline(fileStream1, thisStopWord)) {
        stopWords.push_back(thisStopWord);
    }

    fileStream1.close();

    // handle data file
    ifstream fileStream2(filename);
    if (fileStream2.fail()) {
        return 0;
    }

    int urlCounter = indexHelper1(fileStream2, index, stopWords);
    
    fileStream2.close();

    return urlCounter;
}


int indexHelper1(ifstream &fileStream, map<string, set<string>>& index, vector <string> stopWords) 
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
            if ( stopWords.size() == 0 ) {
                indexHelper2(index, webpageURL, token);
            }
            else if ( stopWords.size() != 0  && (find(stopWords.begin(), stopWords.end(), token) == (stopWords.end())) ) {     // for other index and if not a stop word
                indexHelper2(index, webpageURL, token);
            }
        }

        getline(fileStream, webpageURL);
    }

    return urlCounter;
}


void indexHelper2(map<string, set<string>>& index, string webpageURL, string token) {
    if ( index.find(token) != index.end() ) {
        index.find(token)->second.insert(webpageURL);
    }
    else {
        set <string> setOfURLs;
        setOfURLs.insert(webpageURL);
        index.emplace(token, setOfURLs);
    }
}


// Create and return a new set that has every webpage URL from set1 and every webpage URL from set2 (' ')
set<string> SetUnion(set<string> set1, set<string> set2) 
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


// Create and return a new set that has every webpage URL from set1 which is also in set2 ('+')
set<string> SetIntersection(set<string> set1, set<string> set2) 
{
    set<string> result;
    for (string urlInSet1 : set1) {
        if (set2.find(urlInSet1) != set2.end()) {
            result.insert(urlInSet1);
        }
    }
    return result;
}


// Create and return a new set that has every webpage URL from set1 which is not in set2 ('-')
set<string> SetDifference(set<string> set1, set<string> set2) 
{
    set<string> result;
    for (string urlInSet1 : set1) {
        if (set2.find(urlInSet1) == set2.end()) {
            result.insert(urlInSet1);
        }
    }
    return result;
}


// Parameters: 1) inverted index map abstraction populated in buildIndex(),
//             2) string search term that is non-empty and either a single word or multiple consecutive words, 
//                each of which (besides the first) may be preceded by '+' or '-' as the first character
// Behavior: Use cleanToken() to clean the search query and assume no query will clean to the empty string.
//           For a single search term, matches are the URLs of the webpages containing that term.
//           For a compound search query, process each word left to right:
//           1) If prefaced with '-', remove matches for that term from existing results.
//           2) If prefaced with '+', intersect matches for that term with existing results.
//           3) Otherwise, union matches across search terms or existing results.
// Return value: Set of matched URL strings
set<string> findQueryMatches(map<string, set<string>>& index, string query) 
{
    set<string> result;
    
    query = cleanToken(query);      // also ensures case-insensitive searching

    string wordInQuery;
    stringstream myStringStream (query);

    while (getline (myStringStream, wordInQuery, ' ')) {
        if (wordInQuery[0] == '-') {
            wordInQuery = cleanToken(wordInQuery);
            result = SetDifference(result, index.find(wordInQuery)->second);
        }

        else if (wordInQuery[0] == '+') {
            wordInQuery = cleanToken(wordInQuery);
            result = SetIntersection(result, index.find(wordInQuery)->second);
        }

        else {
            wordInQuery = cleanToken(wordInQuery);
            result = SetUnion(result, index.find(wordInQuery)->second);
        }
    }

    return result;
}


// Parameter: name of file with webpage data
// Behavior: Implement a console program that builds an inverted index from the file data using buildIndex(),
//           prompts the user to enter a search query, finds matches for the query using findQueryMatches(),
//           and prints the matched URLs. Exit the program when the user presses 'enter' (empty string).
void searchEngine(string filename) 
{
    map<string, set<string>> index;
    int totalWebpages = buildIndex(filename, index);
    // buildNoStopWordsIndex (filename, "stopwords.txt", index);

    cout << "Indexed " << totalWebpages << " pages containing " << index.size() << " unique terms" << endl;

    string userQuery;
    cout << "\nEnter query sentence (press enter to quit): ";
    getline (cin, userQuery);

    while (userQuery != "") {
        set<string> result = findQueryMatches(index, userQuery);

        cout << "Found " << result.size() << " matching pages" << endl;

        for (string thisURL : result) {
            cout << thisURL << endl;
        }

        cout << "\nEnter query sentence (press enter to quit): ";
        getline (cin, userQuery);
    }
    
    cout << "Thank you for searching!";
    exit(0);
}



