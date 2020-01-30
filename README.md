# Spellchecker
The following program allows user to enter a word and determine if it is spelled correctly. If it is not, then it will suggest 5 alternatives. The alternatives are calculated using Levenshtein distance. 

https://en.wikipedia.org/wiki/Levenshtein_distance

This program has both a recursive implementation courtesty of RosettaCode (https://rosettacode.org/wiki/Levenshtein_distance#C) and my implementation that is perfromed iteratively using an array for faster compute times. 

The words are stored in a hashmap with buckets.
