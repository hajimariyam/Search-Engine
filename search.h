// TODO: remove and replace this file header comment
// This is a .h file you will edit and turn in.

// Remove starter comments and add your own
// comments on each function and on complex code sections.

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <map>
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string token) 
{
    while (ispunct(token[0])) {
        token.replace(0, 1, "");
    }

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


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
set<string> gatherTokens(string text) 
{
    set<string> tokens;
    
    // tokenize into individual strings by spaces
    string tempString;
    stringstream myStringStream (text);
    while (getline (myStringStream, tempString, ' ')) {
        tempString = cleanToken(tempString);
        if (tempString != "") {
            tokens.insert(tempString);      // store cleaned non-empty tokens in set
        }
    }

    return tokens;
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string filename, map<string, set<string>>& index) 
{
    cout << "Stand by while building index...\n";    

    ifstream fileStream(filename);
    if (fileStream.fail()) {
        return 0;
    }

    int urlCounter = 0;
    string webpageURL;
    string webpageText;

    getline(fileStream, webpageURL);
    while (webpageURL != "") {
        urlCounter++;
        
        getline(fileStream, webpageText);
        set <string> setOfTokens = gatherTokens(webpageText);

        for (string token : setOfTokens) {
            if (index.find(token) != index.end()) {
                index.find(token)->second.insert(webpageURL);
            }
            else {
                set <string> setOfURLs;
                setOfURLs.insert(webpageURL);
                index.emplace(token, setOfURLs);
            }
        }
        
        getline(fileStream, webpageURL);
    }
    
    // return the count of web pages that were processed from the file and added to the index
    return urlCounter;
}


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


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
set<string> findQueryMatches(map<string, set<string>>& index, string query) 
{
    set<string> result;
    
    query = cleanToken(query);

    string tempString;
    stringstream myStringStream (query);

    while (getline (myStringStream, tempString, ' ')) {
        if (tempString[0] == '-') {
            tempString = cleanToken(tempString);
            result = SetDifference(result, index.find(tempString)->second);
        }

        else if (tempString[0] == '+') {
            tempString = cleanToken(tempString);
            result = SetIntersection(result, index.find(tempString)->second);
        }

        else {
            result = SetUnion(result, index.find(tempString)->second);
        }
    }

    return result;
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string filename) 
{
    // construct  inverted index from the contents of the database file
    map<string, set<string>> index;
    int totalWebpages = buildIndex(filename, index);

    // prints how many web pages were processed to build the index and how many distinct words were found across all pages
    cout << "Indexed " << totalWebpages << " pages containing " << index.size() << " unique terms" << endl;

    // enter a loop that prompts the user to enter a query
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

    // for each query entered by the user, it finds the matching pages and prints the URLs
    // the user presses enter (empty string) to indicate they are done and the program finishes executing
}