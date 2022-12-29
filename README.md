# Search Engine

Search engines are one of the most influential developments of the modern internet age, having completely revolutionized the way that people use and interact with the web. What was once an intractable jumble of data with no semblance of organization has been transformed into an easily searchable repository of information. 

This C++ program uses map and set abstractions to build a document search engine that can find webpages with body text that matches a user's query. It is a simplified recreation of the technology underpinning modern search engines today. 

## User Interface

<img src='https://github.com/hajimariyam/Search-Engine/blob/main/gif.gif' width='75%' />

## Details

The magic of search is enabled by pre-processing the body text of each page and storing it as an inverted index into a data structure optimized for lightning-fast retrieval of search results. 

The inverted index can filter out stop words for more productive search results. See the main.cpp header for instructions on how to enable this feature. The default Snowball stoplist used in this program as "stopwords.txt" can be found at: https://github.com/igorbrigadir/stopwords/blob/master/en/snowball_original.txt

Searching is case-insensitive. Queries are processed from left to right and intersected with existing results if prefaced with **+**, removed if **-**, and unioned otherwise. For example:

- ```pointer``` matches all pages containing "pointer"
- ```simple cheap``` means *simple OR cheap*, matches pages that contain either "simple" or "cheap" or both
- ```tasty +healthy``` means *tasty AND healthy*, matches pages that contain both "tasty" and "healthy"
- ```tasty -mushrooms``` means *tasty WITHOUT mushrooms*,	matches pages that contain "tasty" but do not contain "mushrooms"
- ```tasty -mushrooms simple +cheap```	means *tasty WITHOUT mushrooms OR simple AND cheap*, matches pages that match ((("tasty" without "mushrooms") or "simple") and "cheap")

## Data File Format

The document being searched should be in the 'datafiles' folder. The content of the the document is grouped into 2-line pairs with the following structure:
- **Line 1:** webpage URL
- **Line 2:** webpage body text (in a single string without newlines)

For example:

```
https://www.cplusplus.com/reference/regex/ssub_match/
This is an instantiation of the sub_match class template for matches on string objects.The members of this class are those described in sub_match, but using string::const_iterator as its BidirectionalIterator template parameter.Please, refer to sub_match for further information.
https://www.cplusplus.com/reference/chrono/steady_clock/
Clock classes provide access to the current time_point.steady_clock is specifically designed to calculate time intervals.
```

## Further Developments
 
- Weighted search results, sorted by most relevant to least relevant. A simple approach is to re-think the index to include the number of matches and give a higher rank to pages with more occurrences of the search term.

- Multi-word search, by keeping track of where in each document a word is to determine if words in a search phrase are next to each other in any particular document.

- Stemming, or reducing words to their base form. For example, "section" and "sections" would become "section" in the index.

