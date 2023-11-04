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

int countWordsOfLength(char* filename, int wordSize) { 

    FILE* dictionary = fopen(filename, "r");

    if (!dictionary){
        return -1;
    }
    
    int count = 0;
    char character = '\0';

    while (!feof(dictionary)){
        int length = 0;

        fscanf(dictionary, "%c", &character);
        while (!isspace(character) && !feof(dictionary)){
            fscanf(dictionary, "%c", &character);
            length++;
        }

        if (length == wordSize){
            count++;
        }

    }

    fclose(dictionary);

    return count;
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
   
    FILE* dictionary = fopen(filename, "r");

    if (!dictionary){
        return false;
    }

    int arrLen = 0;
    char currWord[50];
    while (!feof(dictionary)){
        fscanf(dictionary, "%s", currWord);

        if (strlen(currWord) == wordSize){
            strcpy(words[arrLen], currWord);
            arrLen++;
        }

    }

    fclose(dictionary);

    if (arrLen == numWords){
        return true;
    }

    return false;
}

int findWord(char** words, char* aWord, int loInd, int hiInd) { 

    int midInd = (loInd + hiInd) / 2;

    if (strcmp(words[midInd], aWord) == 0){
        return midInd;
    }
    else if (loInd > hiInd){
        return -1;
    }
    else{
        if (strcmp(aWord, words[midInd]) < 0){
            return findWord(words, aWord, loInd, midInd - 1);
        }
        else{
            return findWord(words, aWord, midInd + 1, hiInd);
        }
    }

}

void freeWords(char** words, int numWords) {

    for (int i = 0; i < numWords; i++){
        free(words[i]);
    }

    free(words);

}

void insertWordAtFront(WordNode** ladder, char* newWord) {

    WordNode* newNode = malloc(sizeof(WordNode));
    newNode->myWord = newWord;
    newNode->next = *ladder;
    *ladder = newNode;

}

int getLadderHeight(WordNode* ladder) {

    if (!ladder){
        return 0;
    }
    else{
        return 1 + getLadderHeight(ladder->next);
    }

}

WordNode* copyLadder(WordNode* ladder) {

    if (!ladder){
        return NULL; 
    }

    WordNode* newHead = malloc(sizeof(WordNode));
    newHead->myWord = ladder->myWord;
    newHead->next = NULL;
    WordNode* current = newHead;
    ladder = ladder->next;

    while (ladder){
        WordNode* temp = malloc(sizeof(WordNode));
        temp->myWord = ladder->myWord;
        temp->next = NULL;

        current->next = temp;
        current = current->next;
        ladder = ladder->next;

    }

    return newHead;
}

void freeLadder(WordNode* ladder) {

    while (ladder){
        WordNode* temp = ladder->next;
        free(ladder);
        ladder = temp;
    }
}

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {

    LadderNode* newNode = malloc(sizeof(LadderNode));
    newNode->topWord = newLadder;
    newNode->next = NULL;

    if (!(*list)){
        (*list) = newNode;
        return;
    }

    LadderNode* current = *list;

    while (current->next){
        current = current->next;
    }

    current->next = newNode;

}

WordNode* popLadderFromFront(LadderNode** list) {

   if (!(*list)){
        return NULL;
    }
    
    WordNode* ladderHead = (*list)->topWord;

    LadderNode* nextLadder = (*list)->next;
    free(*list);
    *list = nextLadder;

    return ladderHead;
}

void freeLadderList(LadderNode* myList) {

    while (myList){

        freeLadder(myList->topWord);

        LadderNode* nextLadder = myList->next;
        free(myList);
        myList = nextLadder;
    }
}

void neighborNotFinal(WordNode* origLadder, char* neighbor, LadderNode** list){

    WordNode* anotherLadder = copyLadder(origLadder);
    insertWordAtFront(&anotherLadder, neighbor);
    insertLadderAtBack(list, anotherLadder);
    
}

WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    LadderNode* myList = NULL;
    WordNode* myLadder = NULL;

    insertWordAtFront(&myLadder, startWord);
    insertLadderAtBack(&myList, myLadder);

    while (myList){
        myLadder = popLadderFromFront(&myList);

        for (int i = 0; i < wordSize; i++){

            for (int j = 0; j < 26; j++){
                char currWord[wordSize + 1];
                strcpy(currWord, myLadder->myWord);
                currWord[i] = 'a' + j;
                int idx = findWord(words, currWord, 0, numWords - 1);


                if (idx != -1){
                    
                    if (usedWord[idx]){
                        continue;
                    }

                    usedWord[idx] = true;

                    if (strcmp(currWord, finalWord) == 0){
                        insertWordAtFront(&myLadder, words[idx]);

                        WordNode* temp = copyLadder(myLadder);

                        freeLadderList(myList);
                        freeLadder(myLadder);
                        return temp;
                    }
                    else{
                        neighborNotFinal(myLadder, words[idx], &myList);
                    }
                }
            }

        }

        freeLadder(myLadder);

    }
    
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

    printf("Word at idx 9311: %s\n", words[9311]);

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
