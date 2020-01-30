#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
This code calculates the Levenshtein distance between two words.
This code was sourced from Rosetta Code.
https://rosettacode.org/wiki/Levenshtein_distance#C
*/

int levenshtein(const char *s, int ls, const char *t, int lt)
{
        int a, b, c;
 
        /* if either string is empty, difference is inserting all chars 
         * from the other
         */
        if (!ls) return lt;
        if (!lt) return ls;
 
        /* if last letters are the same, the difference is whatever is
         * required to edit the rest of the strings
         */
        if (s[ls - 1] == t[lt - 1])
                return levenshtein(s, ls - 1, t, lt - 1);
 
        /* else try:
         *      changing last letter of s to that of t; or
         *      remove last letter of s; or
         *      remove last letter of t,
         * any of which is 1 edit plus editing the rest of the strings
         */
        a = levenshtein(s, ls - 1, t, lt - 1);
        b = levenshtein(s, ls,     t, lt - 1);
        c = levenshtein(s, ls - 1, t, lt    );
 
        if (a > b) a = b;
        if (a > c) a = c;
 
        return a + 1;
}





int levenshtein2(const char *a, int lengthOne, const char *b, int lengthTwo)
{
  if (!lengthOne) return lengthTwo;
  if (!lengthTwo) return lengthOne;

  //String lengths are rows and columns
  int cols = lengthOne;
  int rows = lengthTwo; 


  //printf("%d", rows);
  //printf("\n");
  //printf("%d", cols);
  //printf("\n");

  //Add an extra row and column
  int arr[rows+1][cols+1];

  //Fill the first row with 0 to i
  for(int i=0; i < cols + 1; i++){
    arr[0][i] = i;
  }

  //Fill the first column with 0 to i
  for(int i = 0; i < rows + 1; i++){
    arr[i][0] = i;
  }


  //Fill the rest of the matrix my matching the current letter to the min cost  
  //of the letter above, to the left and to the diagonal. The lowest cost word is then 
  //added to the current cost. In the end the bottom right value is the total LD. 
  int i = 0;
  int j = 0;
  int cost = 0;
  int min = 0;
  for(i = 1; i < rows + 1; i++){

    char firstLetter = a[i-1]; //The current letter to compare to

    for(j = 1; j < cols + 1; j++){

        char secondLetter = b[j-1]; //The letter we will compare with the current letter

        //If the letters match, there is no cost
        if(firstLetter == secondLetter){
          cost = 0;
          //printf("These letters are equal");
        }
        else{
          cost = 1;
        }

        //Index to the left
        int left = arr[i][j-1];
        //Index to the diagonal
        int diag = arr[i-1][j-1];
        //index above
        int above = arr[i-1][j];

        //Find the minimum betwwen the 3 numbers
        if(left <= diag && left <= above){
          min = left;
        }
        else if(diag <= left && diag <= above){
          min = diag;
        }
        else if(above <= left && above <= diag){
          min = above;
        }

        //Calculte the total cost
        arr[i][j] = min + cost;
        
    }

  }


  //Print the array
  /*
  for(int i = 0; i < rows + 1; i++){
    for(int j = 0; j < cols + 1; j++){
        printf("%d", arr[i][j]);
    }
    printf("\n");
  }
  */

  return arr[rows][cols]; //This is the final LD 


    
}

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement
   int end = 1; //Loop control
   char* word = nextWord(file); //The word from each line in the text file

  while(end == 1){
    hashMapPut(map, word, 0); //Add the word to the map
    word = nextWord(file); //Advance to the next word
    //Terminate the loop
    if(word == NULL){
      end = 0;
    }
  }


}

/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    HashMap* map = hashMapNew(1000);

    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
   
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);


        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
        else{

          // Implement the spell checker code here..

          //Check to make sure only letters are entered, by comparing the char values
            int k;
            int valid = 1;
            for(k = 0; k < strlen(inputBuffer); k++){
              if(inputBuffer[k] < 65 || inputBuffer[k] > 122){
                  printf("Invalid string");
                  printf("\n");
                  valid = 0;
                  break;
              }
            }

          if(valid == 1){

            //Convert the string to all lower case here
            int k;
            int lowerCase = 1;
            for(k = 0; k < strlen(inputBuffer); k++){
              if(inputBuffer[k] < 91){
                 inputBuffer[k] += 32; //Convert to Upper Case
                 lowerCase = 0;
              }
            }

            //Only print the new string if it was modified
            if(lowerCase == 0){
              printf("\n");
              printf("New string:");
              printf("%s", inputBuffer);
              printf("\n");
            }
            
            //First do a quick check for the word
            if(hashMapContainsKey(map, inputBuffer)){
              printf("\n");
              printf("%s", "The inputted word is spelled correctly");
              printf("\n");
            }
            //Otherwise we will need to find the closest words
            else{
              //Loop the entire hashMap
              //Generate the Lev distance
              //Store the current top 5 in an array (lowest Lev distance)
              //print the top 5

              int distances[5] = {16,16,16,16,16}; //Set the distances to max char length
              char suggestions[5][16]; //Create an array to hold 5 words of max char length

              int i;
              for(i=0; i< map->capacity; i++){
                if(map->table[i] != NULL){

                  struct HashLink* temp = map->table[i];

                  while(temp != NULL){

                    struct HashLink* next = temp->next;
                    int levDistance = levenshtein2(inputBuffer, strlen(inputBuffer), temp->key, strlen(temp->key));
                    hashMapPut(map, temp->key, levDistance );

                    //Compare the current distances to the new distances
                    int j;
                    for(j = 0; j < 5; j++){
                      if(temp->value < distances[j]){
                        distances[j] = temp->value; //Update the new distance
                        strcpy(suggestions[j], temp->key); //Copy the new string
                        j = 5; //End the loop
                      }
                    }

                  temp = next;
                  
                  }

                }
          
              }
            
              //Print suggestions
              printf("\n");
              printf("%s", "The inputted word is spelled incorrectly");
              printf("\n");
              printf("%s", "Here are some suggestions:");
              printf("\n");
              printf("%s", suggestions[0]); 
              printf("\n");
              printf("%s", suggestions[1]); 
              printf("\n");
              printf("%s", suggestions[2]); 
              printf("\n");
              printf("%s", suggestions[3]); 
              printf("\n");
              printf("%s", suggestions[4]); 
              printf("\n");
            
            }

          }
        }
        
    }

    hashMapDelete(map);
    return 0;
}
