## Word Ladder Application

This is a short program that builds and finds the shortest word ladder between 2 words of the same length.

Project originally based and original files from UIC Programming Practicum course.

## How it works

First, the user is asked to choose a word length and a word list file (several provided)

Then, the program asks for 2 words as the start and target words to build the ladder from.
The program will then attempt to find the shortest possible word ladder by building all possible ladder using a breadth-first approach of checking if each new addition to a word ladders is the target.

## Files

* main.c - The main file of the project. Contains the main functionality of the project.
* program.exe - An compliled executeable of the project
* .txt files - dictionary.txt: full list of words; simple_.txt: short word lists of the same length
