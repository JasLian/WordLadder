/*Main .c file for project. Program finds the shortest possible word ladder between 2 words
Written by Jason Liang*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

/*Funcion counts the number of words in a 
dictionary file of a specific word length
Takes 2 parameters:
    1. filename: the dictionary file name
    2. wordSize: the specific word length
Return the number of words counted*/
int countWordsOfLength(char* filename, int wordSize) { 

    FILE* dictionary = fopen(filename, "r");

    //if file could not be opened, return -1
    if (!dictionary){
        return -1;
    }
    
    int count = 0;
    char currWord[50];

    //reads through the file, word by word
    while (!feof(dictionary)){

        //if a newspace character or eof is reached, that means a full has been read
        fscanf(dictionary, "%s", currWord);

        if (strlen(currWord) == wordSize){
            count++; //if the word length matches, increment the count of words
        }

    }

    //closes the file and return count
    fclose(dictionary);

    return count;
}

/*Function populates an array words of a specific word length from a dictionary file 
Takes 4 parameters:
    1. filename: the dictionary file name
    2. words: an array of pointers to C-strings, array to be population
    3. numWords: the size of the words array
    2. wordSize: the specific word length
Returns a boolean value whether array was populated or not*/
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
   
    FILE* dictionary = fopen(filename, "r");

    //if the file could not be opened, return false
    if (!dictionary){
        return false;
    }

    int arrLen = 0;

    //reads through the file, word by word
    char currWord[50];
    while (!feof(dictionary)){

        fscanf(dictionary, "%s", currWord);

        //if the length of currWord matches wordSize, it is copied and stored in the words array
        if (strlen(currWord) == wordSize){
            strcpy(words[arrLen], currWord);
            arrLen++;
        }

    }

    fclose(dictionary);

    //checks if the array stores numWords words
    if (arrLen == numWords){
        return true;
    }

    return false;
}

/*Fucntion performs binary search to find a specific word in a sorted array of words
Takes 4 parameters:
    1. words: the array of words
    2. aWord: the target word being searched for
    3. loInd: the lower bound for binary search
    4. hiInd: the upper bound for binary search
Returns the index where the word is found*/
int findWord(char** words, char* aWord, int loInd, int hiInd) { 

    int midInd = (loInd + hiInd) / 2; //calculates the middle index

    if (loInd > hiInd){
        return -1; //if loInd and hiInd crosses in values, the word is not in the array
    }
    else if (strcmp(words[midInd], aWord) == 0){
        return midInd; //returns the index if aWord is found
    }
    else{

        //recursive cases:

        //searches the lower half of the array
        if (strcmp(aWord, words[midInd]) < 0){
            return findWord(words, aWord, loInd, midInd - 1);
        }

        //searches the upper half of the array
        else{
            return findWord(words, aWord, midInd + 1, hiInd);
        }

    }

}


/*Function frees a heap allocated array of pointers to heap allocated C-strings
Takes in 2 parameters:
    1. words: the array being freed
    2. numWords: the length of the array
No returns*/
void freeWords(char** words, int numWords) {

    //iterates through the arary and free each string
    for (int i = 0; i < numWords; i++){
        free(words[i]);
    }

    free(words); //free the array itself

}

/*Function prepends a new word to a linked list of WordNode structs
Takes 2 parameters:
    1. ladder: the head of the ladder/linked list
    2. newWord: the word being prepended
No returns*/
void insertWordAtFront(WordNode** ladder, char* newWord) {

    //creates a new node, assigns its myWord with newWord
    WordNode* newNode = malloc(sizeof(WordNode));
    newNode->myWord = newWord;

    //links newNode to the front
    newNode->next = *ladder;
    *ladder = newNode;

}

/*Function calculates the height of a word ladder, by finding the length
of the ladder as a linked list
Takes 1 parameter:
    1. ladder: the head of the ladder/linked list
Returns the height*/
int getLadderHeight(WordNode* ladder) {

    //base case if a null node is reached
    if (!ladder){
        return 0;
    }

    //recursive case
    else{

        //recursive traverses the ladder, counting and summing the number of nodes
        return 1 + getLadderHeight(ladder->next);
    }

}

/*Function copies and returns a word ladder
Takes 1 parameter:
    1. ladder: the head of the ladder/linked list
Returns a pointer to the new ladder copy*/
WordNode* copyLadder(WordNode* ladder) {

    //if the ladder is empty, return NULL
    if (!ladder){
        return NULL; 
    }

    //creates a new head node, and sets its myWord to ladder's myWord
    WordNode* newHead = malloc(sizeof(WordNode));
    newHead->myWord = ladder->myWord;
    newHead->next = NULL;

    //traverses through the original ladder
    WordNode* current = newHead;
    ladder = ladder->next;
    while (ladder){

        //create a new node, assign its myWord, and links it with the new ladder
        WordNode* temp = malloc(sizeof(WordNode));
        temp->myWord = ladder->myWord;
        temp->next = NULL;

        //move on to the next nodes
        current->next = temp;
        current = current->next;
        ladder = ladder->next;

    }

    return newHead; //returns the head to the new ladder
}

/*Function frees a word ladder
Takes 1 parameter:
    1. ladder: the head of the ladder/linked list
No returns*/
void freeLadder(WordNode* ladder) {

    //traverses the ladder, freeing each node
    while (ladder){
        WordNode* temp = ladder->next;
        free(ladder);
        ladder = temp;
    }
}

/*Function appends a new ladder to the back of the list of ladders
Takes 2 parameters:
    1. list: a linked list of LadderNode structs representing the list of ladders
    2. newLadder: the new word ladder
No returns*/
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {

    //creates a new node to append, set topWord to newLadder
    LadderNode* newNode = malloc(sizeof(LadderNode));
    newNode->topWord = newLadder;
    newNode->next = NULL;

    //for case if appending to empty list
    if (!(*list)){
        (*list) = newNode;
        return;
    }

    //traverse to the end of the list
    LadderNode* current = *list;
    while (current->next){
        current = current->next;
    }

    //appends newNode
    current->next = newNode;

}

/*Function pops the first ladder off of the list of ladders
Takes 1 parameter:
    1. list: a linked list of LadderNode structs representing the list of ladders
Returns the word ladder that was popped off*/
WordNode* popLadderFromFront(LadderNode** list) {

    //if the list is empty
    if (!(*list)){
        return NULL;
    }

    //stores the word ladder
    WordNode* ladderHead = (*list)->topWord;

    //pop and free the node from the list
    LadderNode* nextLadder = (*list)->next;
    free(*list);
    *list = nextLadder;

    return ladderHead;

}

/*Function frees the entire list of word ladders
Takes 1 parameter:
    1. list: a linked list of LadderNode structs representing the list of ladders
No returns*/
void freeLadderList(LadderNode* myList) {

    //traverses the list, freeing each node and its word ladder
    while (myList){

        freeLadder(myList->topWord);

        LadderNode* nextLadder = myList->next;
        free(myList);
        myList = nextLadder;

    }
}

/*Function is a helper function, used to copy a ladder, insert the neighbor,
and insert the copy to the back of the list of ladders
Takes 3 parameters:
    1. origLadder: the original word ladder
    2. neighbor: the neighbor word to be inserted
    3. list: the list of ladders
No returns*/
void neighborNotFinal(WordNode* origLadder, char* neighbor, LadderNode** list){

    WordNode* anotherLadder = copyLadder(origLadder);
    insertWordAtFront(&anotherLadder, neighbor);
    insertLadderAtBack(list, anotherLadder);
    
}

/*Function finds the shortest word ladder between 2 words through BFS
Takes 6 parameters:
    1. words: the array of words
    2. usedWord: an array of boolean values determining if a word has already been used
    3. numWords: the length of the words array
    4. wordSize: the word length of each word
    5. startWord: the first word in the ladder
    6. finalWord: the last word the word ladder should reach
Returns the shortest ladder found*/
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    LadderNode* myList = NULL;
    WordNode* myLadder = NULL;

    //first creates a ladder with just startWord inserted, and inserts ladder
    insertWordAtFront(&myLadder, startWord);
    insertLadderAtBack(&myList, myLadder);

    //while myList is not empty, continue to build and search for the shortest ladder
    while (myList){
        myLadder = popLadderFromFront(&myList); //pop the ladder and stores it

        //nested loop that searches through all possible neighbor words of the current top word of the ladder
        for (int i = 0; i < wordSize; i++){

            for (int j = 0; j < 26; j++){
                char currWord[wordSize + 1];
                strcpy(currWord, myLadder->myWord);
                currWord[i] = 'a' + j; //neighbor words are found by changing each letter one at a time
                int idx = findWord(words, currWord, 0, numWords - 1);

                //if a neighbor word is found, attempt to add it to current ladder
                if (idx != -1){
                    
                    //the neighbor is checked if it is has already been used
                    if (usedWord[idx]){
                        continue;
                    }

                    usedWord[idx] = true;

                    //if the neighbor is finalWord, return a ladder with it inserted
                    if (strcmp(currWord, finalWord) == 0){
                        insertWordAtFront(&myLadder, words[idx]);

                        WordNode* temp = copyLadder(myLadder); //creates a copy to return

                        //frees list and the ladder
                        freeLadderList(myList);
                        freeLadder(myLadder);

                        return temp;
                    }

                    //otherwise, add the ladder to the list
                    else{
                        neighborNotFinal(myLadder, words[idx], &myList);
                    }
                }
            }

        }

        //myLadder needs to be freed once all possible next ladders are built
        freeLadder(myLadder);

    }
    
    //if no ladders are in myList, no word ladder between startWord and finalWord is possible
    return NULL;
}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
